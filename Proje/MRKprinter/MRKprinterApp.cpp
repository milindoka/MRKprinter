/***************************************************************
 * Name:      MRKprinterApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Milind Oka (oak444@gmail.com)
 * Created:   2017-01-23
 * Copyright: Milind Oka (https://github.com/milindoka/MRKprinter)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "MRKprinterApp.h"
#include "MRKprinterMain.h"

IMPLEMENT_APP(MRKprinterApp);


wxString marklistoncommandline;


bool MRKprinterApp::OnInit()
{

    MRKprinterDialog* dlg = new MRKprinterDialog(0L, _("wxWidgets Application Template"));
    marklistoncommandline=wxGetApp().argv[1];
    dlg->Show();
    return true;
}
