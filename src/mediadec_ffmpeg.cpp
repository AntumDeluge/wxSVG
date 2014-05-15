/////////////////////////////////////////////////////////////////////////////
// Name:        mediadec_ffmpeg.cpp
// Purpose:     FFMPEG Media Decoder
// Author:      Alex Thuering
// Created:     21.07.2007
// RCS-ID:      $Id: mediadec_ffmpeg.cpp,v 1.27 2014-05-15 19:52:58 ntalex Exp $
// Copyright:   (c) Alex Thuering
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#define __STDC_CONSTANT_MACROS
#define __STDC_LIMIT_MACROS

#include <wxSVG/mediadec_ffmpeg.h>
#include <wx/wx.h>
#include <errno.h>

#ifndef INT64_C
#define INT64_C(val) val##LL
#define UINT64_C(val) val##ULL
#endif
extern "C" {
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
#include <libavutil/mathematics.h>
}

wxFfmpegMediaDecoder::wxFfmpegMediaDecoder(): m_formatCtx(NULL), m_videoStream(-1), m_codecCtx(NULL), m_frame(NULL),
		m_width(0), m_height(0) {
	// nothing to do
}

wxFfmpegMediaDecoder::~wxFfmpegMediaDecoder() {
    Close();
}

void wxFfmpegMediaDecoder::Init() {
    av_register_all();
}

void PrintError(const wxString& msg, int err) {
	char errbuf[128];
	const char *errbuf_ptr = errbuf;
	
	if (av_strerror(err, errbuf, sizeof(errbuf)) < 0)
		errbuf_ptr = strerror(AVUNERROR(err));
	wxString errorStr(errbuf_ptr, wxConvUTF8);
	wxLogError(msg + wxT(": ") + errorStr);
}

bool wxFfmpegMediaDecoder::Load(const wxString& fileName) {
	Close();
	if ((fileName.Find(wxT(':')) <= 0 || fileName.Find(wxT(':')) >= 8
			|| fileName.GetChar(fileName.Find(wxT(':')) - 1) == wxT('\\'))
			&& !wxFileExists(fileName)) {
		wxLogError(wxT("%s: no such file or directory"), fileName.c_str());
		return false;
	}
#ifdef __WXMSW__
	int err = avformat_open_input(&m_formatCtx, fileName.utf8_str(), NULL, NULL);
#else
	int err = avformat_open_input(&m_formatCtx, fileName.mb_str(), NULL, NULL);
#endif
	if (err != 0) {
		PrintError(fileName, err);
		return false;
	}
	// Retrieve stream information
	if (avformat_find_stream_info(m_formatCtx, NULL) < 0) {
		wxLogError(wxT("%s: could not find codec parameters"), fileName.c_str());
		return false;
	}
	return true;
}

void wxFfmpegMediaDecoder::Close() {
	EndDecode();
	if (m_formatCtx)
		avformat_close_input(&m_formatCtx);
}

unsigned int wxFfmpegMediaDecoder::GetStreamCount() {
	return m_formatCtx != NULL ? m_formatCtx->nb_streams : 0;
}

wxSize wxFfmpegMediaDecoder::GetVideoSize() {
    OpenVideoDecoder();
    return m_codecCtx ? wxSize(m_codecCtx->width, m_codecCtx->height) : wxSize();
}

float wxFfmpegMediaDecoder::GetFrameAspectRatio() {
	if (m_formatCtx == NULL)
		return -1;
	float frame_aspect_ratio = -1;
	for (unsigned int i = 0; i < m_formatCtx->nb_streams; i++) {
		AVStream *st = m_formatCtx->streams[i];
		AVCodecContext *enc = st->codec;
		if (enc->codec_type == AVMEDIA_TYPE_VIDEO) {
			if (st->sample_aspect_ratio.num)
				frame_aspect_ratio = av_q2d(st->sample_aspect_ratio);
			else if (enc->sample_aspect_ratio.num)
				frame_aspect_ratio = av_q2d(enc->sample_aspect_ratio);
			else
				frame_aspect_ratio = 1;
			frame_aspect_ratio *= (float) enc->width / enc->height;
			break;
		}
	}
	return frame_aspect_ratio;
}

float wxFfmpegMediaDecoder::GetFps() {
	if (m_formatCtx == NULL)
		return -1;
	float result = -1;
	for (unsigned int i = 0; i < m_formatCtx->nb_streams; i++) {
		AVStream* st = m_formatCtx->streams[i];
		if (st->codec->codec_type == AVMEDIA_TYPE_VIDEO && st->r_frame_rate.num && st->r_frame_rate.den) {
			if (st->r_frame_rate.num > st->r_frame_rate.den)
				result = ((float) st->r_frame_rate.num) / st->r_frame_rate.den;
			else
				result = ((float) st->r_frame_rate.den) / st->r_frame_rate.num;
			break;
		}
	}
	return result;
}

StreamType wxFfmpegMediaDecoder::GetStreamType(unsigned int streamIndex) {
	if (m_formatCtx == NULL || streamIndex >= m_formatCtx->nb_streams)
		return stUNKNOWN;
	switch (m_formatCtx->streams[streamIndex]->codec->codec_type) {
	case AVMEDIA_TYPE_VIDEO:
		return stVIDEO;
	case AVMEDIA_TYPE_AUDIO:
		return stAUDIO;
	case AVMEDIA_TYPE_SUBTITLE:
		return stSUBTITLE;
	default:
		break;
	}
	return stUNKNOWN;
}

wxString wxFfmpegMediaDecoder::GetCodecName(unsigned int streamIndex) {
	char buf[256];
	avcodec_string(buf, sizeof(buf), m_formatCtx->streams[streamIndex]->codec, false);
	wxString name = wxString(buf, wxConvLocal).BeforeFirst(wxT(','));
	return name.Index(wxT(':')) > 0 ? name.AfterFirst(wxT(':')).Trim(false) : name;
}

int wxFfmpegMediaDecoder::GetChannelNumber(unsigned int streamIndex) {
	return m_formatCtx ? m_formatCtx->streams[streamIndex]->codec->channels : -1;
}

int wxFfmpegMediaDecoder::GetSampleRate(unsigned int streamIndex) {
	return m_formatCtx ? m_formatCtx->streams[streamIndex]->codec->sample_rate : -1;
}

int wxFfmpegMediaDecoder::GetBitrate(unsigned int streamIndex) {
	return m_formatCtx ? m_formatCtx->streams[streamIndex]->codec->bit_rate : -1;
}

double wxFfmpegMediaDecoder::GetDuration() {
	return m_formatCtx != NULL && m_formatCtx->duration != (int64_t)AV_NOPTS_VALUE ?
			((double)m_formatCtx->duration) / AV_TIME_BASE : -1;
}

bool wxFfmpegMediaDecoder::OpenVideoDecoder() {
	if (m_codecCtx)
		return true;
	// find the first video stream
	m_videoStream = -1;
	for (int i=0; i<(int)m_formatCtx->nb_streams; i++) {
		if (m_formatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			m_videoStream = i;
			break;
		}
	}
	if(m_videoStream == -1)
		return false;
	// get a pointer to the codec context for the video stream 
	m_codecCtx = m_formatCtx->streams[m_videoStream]->codec;
	// find and open the decoder for the video stream 
	AVCodec* codec = avcodec_find_decoder(m_codecCtx->codec_id);
	if (!codec || avcodec_open2(m_codecCtx, codec, NULL) < 0) {
		m_codecCtx = NULL;
		return false;
	}
	return true;
}

void wxFfmpegMediaDecoder::CloseVideoDecoder()
{
    if (m_codecCtx)
        avcodec_close(m_codecCtx); // close the codec
    m_codecCtx = NULL;
}

bool wxFfmpegMediaDecoder::BeginDecode(int width, int height)
{
    if (!m_formatCtx || !OpenVideoDecoder())
        return false;
    // video size
    if (width == -1 || height == -1)
    {
        m_width = m_codecCtx->width;
        m_height = m_codecCtx->height;
    }
    else
    {
        int w = m_codecCtx->width * height / m_codecCtx->height;
        int h = m_codecCtx->height * width / m_codecCtx->width;
        m_width = w < width ? w : width;
        m_height = w < width ? height : h;
    }
    // allocate video frame
#if LIBAVCODEC_VERSION_MAJOR >= 55
    m_frame = av_frame_alloc();
#else
    m_frame = avcodec_alloc_frame();
#endif
    if (!m_frame) {
        avcodec_close(m_codecCtx);
        m_codecCtx = NULL;
        return false;
    }
    return true;
}

bool wxFfmpegMediaDecoder::SetPosition(double pos, bool keyFrame) {
    if (m_formatCtx == NULL)
        return false;
    int64_t timestamp = (int64_t) (pos * AV_TIME_BASE);
    if (m_formatCtx->start_time != (int64_t)AV_NOPTS_VALUE)
        timestamp += m_formatCtx->start_time;
    return av_seek_frame(m_formatCtx, -1, timestamp,
    		keyFrame ? AVSEEK_FLAG_BACKWARD : AVSEEK_FLAG_ANY|AVSEEK_FLAG_BACKWARD) >= 0;
}

double wxFfmpegMediaDecoder::GetPosition() {
	if (m_formatCtx == NULL || m_formatCtx->iformat == NULL)
		return -1;
	AVStream *st = m_formatCtx->streams[m_videoStream];
	int64_t timestamp = st->cur_dts;
	if (timestamp == (int64_t)AV_NOPTS_VALUE)
		return -1;
	timestamp = av_rescale(timestamp, AV_TIME_BASE * (int64_t)st->time_base.num, st->time_base.den);
	if (m_formatCtx->start_time != (int64_t)AV_NOPTS_VALUE)
		timestamp -= m_formatCtx->start_time;
	return ((double)timestamp)/AV_TIME_BASE;
}

wxImage wxFfmpegMediaDecoder::GetNextFrame() {
	if (!m_frame && !BeginDecode())
		return wxImage();

	int frameFinished;
	AVPacket packet;
	 while (av_read_frame(m_formatCtx, &packet) >=0) {
		// is this a packet from the video stream?
		if (packet.stream_index == m_videoStream) {
			// decode video frame
			avcodec_decode_video2(m_codecCtx, m_frame, &frameFinished, &packet);
			if (frameFinished) {
				SwsContext* imgConvertCtx = sws_getContext(m_codecCtx->width, m_codecCtx->height, m_codecCtx->pix_fmt,
						m_width, m_height, PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);
				if (imgConvertCtx == NULL)
					return wxImage();

				wxImage img(m_width, m_height);
				uint8_t *rgbSrc[3] = { img.GetData(), NULL, NULL };
				int rgbStride[3] = { 3 * m_width, 0, 0 };
				sws_scale(imgConvertCtx, m_frame->data, m_frame->linesize, 0, m_codecCtx->height, rgbSrc, rgbStride);
				av_free_packet(&packet);
				return img;
			}
		}
		// free the packet that was allocated by av_read_frame
		av_free_packet(&packet);
	}
	return wxImage();
}

void wxFfmpegMediaDecoder::EndDecode()
{
    if (m_frame)
        av_free(m_frame);
    m_frame = NULL;
    CloseVideoDecoder();
}
