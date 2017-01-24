/***************************************************************
 * Name:      MRKprinterMain.h
 * Purpose:   Defines Application Frame
 * Author:    Milind Oka (oak444@gmail.com)
 * Created:   2017-01-23
 * Copyright: Milind Oka (https://github.com/milindoka/MRKprinter)
 * License:
 **************************************************************/

#ifndef MRKPRINTERMAIN_H
#define MRKPRINTERMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "MRKprinterApp.h"


#include <wx/button.h>
#include <wx/statline.h>
class MRKprinterDialog: public wxDialog
{
    public:
        MRKprinterDialog(wxDialog *dlg, const wxString& title);
        void GetAllMrkFiles();
        int Print();
        ~MRKprinterDialog();

    protected:
        enum
        {
            idBtnQuit = 1000,
            idBtnAbout
        };
        wxStaticText* m_staticText1;
        wxButton* BtnAbout;
        wxStaticLine* m_staticline1;
        wxButton* BtnQuit;

    private:
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        DECLARE_EVENT_TABLE()
};

#endif // MRKPRINTERMAIN_H
