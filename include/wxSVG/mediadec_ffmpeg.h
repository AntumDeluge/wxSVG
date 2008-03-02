/////////////////////////////////////////////////////////////////////////////
// Name:        mediadec_ffmpeg.h
// Purpose:     FFMPEG Media Decoder
// Author:      Alex Thuering
// Created:     21.07.2007
// RCS-ID:      $Id: mediadec_ffmpeg.h,v 1.2 2008-03-02 18:42:57 ntalex Exp $
// Copyright:   (c) Alex Thuering
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef FFMPEG_MEDIA_DECODER_H
#define FFMPEG_MEDIA_DECODER_H

#include <wx/string.h>
#include <wx/image.h>
struct AVFormatContext;
struct AVCodecContext;
struct AVFrame;
struct AVStream;

class wxFfmpegMediaDecoder
{
  public:
    wxFfmpegMediaDecoder();
    virtual ~wxFfmpegMediaDecoder();
    static void Init();
    
    virtual bool Load(const wxString& fileName);
    virtual void Close();
    
    double GetDuration();
    bool SetPosition(double pos);
    double GetPosition();
    
    virtual wxSize GetVideoSize();
    
    virtual bool BeginDecode(int width = -1, int height = -1);
    virtual wxImage GetNextFrame();
    virtual void EndDecode();
    
  private:
    AVFormatContext* m_formatCtx;
    int m_videoStream;
    AVCodecContext* m_codecCtx;
    AVFrame* m_frame;
    int m_width;
    int m_height;
    bool OpenVideoDecoder();
    void CloseVideoDecoder();
};

#endif //FFMPEG_MEDIA_DECODER_H
