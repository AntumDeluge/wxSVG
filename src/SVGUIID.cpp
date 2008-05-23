//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUIElement.h
// Purpose:     
// Author:      Laurent Bessard
// Created:     2008/04/30
// RCS-ID:      $Id: SVGUIID.cpp,v 1.1 2008-05-23 13:47:53 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGUIID.h"

// --------------- SVGUIID implementation taken from wx/contrib/src/xrc/xmlres.cpp-----------------------------

#define SVGUIID_TABLE_SIZE     1024


struct SVGUIID_record
{
  int id;
  wxChar *key;
  SVGUIID_record *next;
};

static SVGUIID_record *SVGUIID_Records[SVGUIID_TABLE_SIZE] = {NULL};

static int SVGUIID_Lookup(const wxChar *str_id, int value_if_not_found = -2)
{
  static int SVGUIID_LastID = wxID_HIGHEST;

  int index = 0;

  for (const wxChar *c = str_id; *c != wxT('\0'); c++) index += (int)*c;
  index %= SVGUIID_TABLE_SIZE;

  SVGUIID_record *oldrec = NULL;
  int matchcnt = 0;
  for (SVGUIID_record *rec = SVGUIID_Records[index]; rec; rec = rec->next)
  {
    if (wxStrcmp(rec->key, str_id) == 0)
      return rec->id;
    matchcnt++;
    oldrec = rec;
  }

  SVGUIID_record **rec_var = (oldrec == NULL) ?
                            &SVGUIID_Records[index] : &oldrec->next;
  *rec_var = new SVGUIID_record;
  (*rec_var)->key = wxStrdup(str_id);
  (*rec_var)->next = NULL;

  wxChar *end;
  if (value_if_not_found != -2)
    (*rec_var)->id = value_if_not_found;
  else
  {
    int asint = wxStrtol(str_id, &end, 10);
    if (*str_id && *end == 0)
    {
      // if str_id was integer, keep it verbosely:
      (*rec_var)->id = asint;
    }
    else
      (*rec_var)->id = ++SVGUIID_LastID;
  }

  return (*rec_var)->id;
}

int GetSVGUIID(const wxChar *str_id)
{
  return SVGUIID_Lookup(str_id);
}

// TODO :  use this functions !!
/*
static void CleanSVGUIID_Record(SVGUIID_record *rec)
{
  if (rec)
  {
    CleanSVGUIID_Record(rec->next);
    free(rec->key);
    delete rec;
  }
}

static void CleanSVGUIID_Records()
{
  for (int i = 0; i < SVGUIID_TABLE_SIZE; i++)
  {
    CleanSVGUIID_Record(SVGUIID_Records[i]);
    SVGUIID_Records[i] = NULL;
  }
}
*/
