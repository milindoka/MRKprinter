/***************************************************************
 * Name:      MRKprinterMain.cpp
 * Purpose:   Code for Application Frame
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

#include <wx/stdpaths.h>
#include <wx/dir.h>



#include <wx/cmdline.h>
#include <wx/app.h>
#include "MRKprinterMain.h"



//IMPLEMENT_APP(MRKprinterApp)
//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}


BEGIN_EVENT_TABLE(MRKprinterDialog, wxDialog)
    EVT_CLOSE(MRKprinterDialog::OnClose)
    EVT_BUTTON(idBtnQuit, MRKprinterDialog::OnQuit)
    EVT_BUTTON(idBtnAbout, MRKprinterDialog::OnAbout)
END_EVENT_TABLE()

MRKprinterDialog::MRKprinterDialog(wxDialog *dlg, const wxString &title)
    : wxDialog(dlg, -1, title)
{
    this->SetSizeHints(wxDefaultSize, wxDefaultSize);
    wxBoxSizer* bSizer1;
    bSizer1 = new wxBoxSizer(wxHORIZONTAL);
    m_staticText1 = new wxStaticText(this, wxID_ANY, wxT("Welcome To\nwxWidgets"), wxDefaultPosition, wxDefaultSize, 0);
    m_staticText1->SetFont(wxFont(20, 74, 90, 90, false, wxT("Arial")));
    bSizer1->Add(m_staticText1, 0, wxALL|wxEXPAND, 5);
    wxBoxSizer* bSizer2;
    bSizer2 = new wxBoxSizer(wxVERTICAL);
    BtnAbout = new wxButton(this, idBtnAbout, wxT("&About"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer2->Add(BtnAbout, 0, wxALL, 5);
    m_staticline1 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    bSizer2->Add(m_staticline1, 0, wxALL|wxEXPAND, 5);
    BtnQuit = new wxButton(this, idBtnQuit, wxT("&Quit"), wxDefaultPosition, wxDefaultSize, 0);
    bSizer2->Add(BtnQuit, 0, wxALL, 5);
    bSizer1->Add(bSizer2, 1, wxEXPAND, 5);
    this->SetSizer(bSizer1);
    this->Layout();
    bSizer1->Fit(this);
}


MRKprinterDialog::~MRKprinterDialog()
{
}

void MRKprinterDialog::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void MRKprinterDialog::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void MRKprinterDialog::OnAbout(wxCommandEvent &event)
{
    //wxString msg = wxbuildinfo(long_f);
    extern wxString marklistoncommandline;
    wxStandardPaths std;
    wxString exePath = std.GetExecutablePath();


    wxMessageBox(marklistoncommandline, _("Welcome to..."));
   // GetAllMrkFiles();

   Print();
    //wxString  par2(wxGetApp().argv[0]);
  //  wxMessageBox(par2, _("Welcome to..."));
//    wxString cla=wxGetApp().argv[0];  //'(exePath, _("Welcome to..."));

}






///////////////////////////////////////////////

#include <iostream>
#include "wx/wx.h"
#include "wx/print.h"
#include "wx/printdlg.h"
#include <cmath>

static const int brush_size = 3;

/**
  * Shows a basic example of how to print stuff in wx.
  */
class QuickPrint : public wxPrintout
{
    wxPageSetupDialogData m_page_setup;

    /** the type of paper (letter, A4, etc...) */
    wxPaperSize m_paper_type;

    /** horizontal or vertical */
    int m_orient;

    // Or, if you use wxWidgets 2.9+ :
    // wxPrintOrientation m_orient;

    /** number of pages we want to print. here it's static, but in a real example you will often
      * want to calculate dynamically once you know the size of the printable area from page setup
      */
    int m_page_amount;

    /** margins, in millimeters */
    int m_margin_left, m_margin_right, m_margin_top, m_margin_bottom;

    /** we'll use this to determine the coordinate system; it describes the number of units per
      * centimeter (i.e. how fine the coordinate system is)
      */
    float m_units_per_cm;

    /** will contain the dimensions of the coordinate system, once it's calculated.
      * in the printing callback, you can then draw from (0, 0) to (m_coord_system_width, m_coord_system_height),
      * which will be the area covering the paper minus the margins
      */
    int m_coord_system_width, m_coord_system_height;

public:

    /**
      * @param page_amount    number of pages we want to print. Here it's static because it's just a test, but in
      *                       real code you will often want to calculate dynamically once you know the size of the
      *                       printable area from page setup
      * @param title          name of the print job / of the printed document
      * @param units_per_cem  we'll use this to determine the coordinate system; it describes the number of units
      *                       per centimeter (i.e. how fine the coordinate system is)
      */
    QuickPrint(int page_amount, wxString title, float units_per_cm) : wxPrintout( title )
    {
        m_page_amount = page_amount;

        m_orient = wxPORTRAIT; // wxPORTRAIT, wxLANDSCAPE
        m_paper_type = wxPAPER_LETTER;
        m_margin_left   = 16;
        m_margin_right  = 16;
        m_margin_top    = 32;
        m_margin_bottom = 32;

        m_units_per_cm   = units_per_cm;
    }


    /** shows the page setup dialog, OR sets up defaults */
    bool performPageSetup(const bool showPageSetupDialog)
    {
        // don't show page setup dialog, use default values
        wxPrintData printdata;
        printdata.SetPrintMode( wxPRINT_MODE_PRINTER );
        printdata.SetOrientation( m_orient );
        printdata.SetNoCopies(1);
        printdata.SetPaperId( m_paper_type );

        m_page_setup = wxPageSetupDialogData(printdata);
        m_page_setup.SetMarginTopLeft    (wxPoint(m_margin_left,  m_margin_top));
        m_page_setup.SetMarginBottomRight(wxPoint(m_margin_right, m_margin_bottom));

        if (showPageSetupDialog)
        {
            wxPageSetupDialog dialog( NULL, &m_page_setup );
            if (dialog.ShowModal() == wxID_OK)
            {

                m_page_setup = dialog.GetPageSetupData();
                m_orient = m_page_setup.GetPrintData().GetOrientation();
                m_paper_type = m_page_setup.GetPrintData().GetPaperId();

                wxPoint marginTopLeft = m_page_setup.GetMarginTopLeft();
                wxPoint marginBottomRight = m_page_setup.GetMarginBottomRight();
                m_margin_left   = marginTopLeft.x;
                m_margin_right  = marginBottomRight.x;
                m_margin_top    = marginTopLeft.y;
                m_margin_bottom = marginBottomRight.y;
            }
            else
            {
                std::cout << "user canceled at first dialog" << std::endl;
                return false;
            }
        }
        return true;
    }

    /** Called when printing starts */
    void OnBeginPrinting()
    {
        // set-up coordinate system however we want, we'll use it when drawing

        // take paper size and margin sizes into account when setting up coordinate system
        // so that units are "square" (1 unit x is a wide as 1 unit y is high)
        // (actually, if we don't make it square, on some platforms wx will even resize your
        //  margins to make it so)
        wxSize paperSize = m_page_setup.GetPaperSize();  // in millimeters

        // still in millimeters
        float large_side = std::max(paperSize.GetWidth(), paperSize.GetHeight());
        float small_side = std::min(paperSize.GetWidth(), paperSize.GetHeight());

        float large_side_cm = large_side / 10.0f;  // in centimeters
        float small_side_cm = small_side / 10.0f;  // in centimeters

        if (m_orient == wxPORTRAIT)
        {
            float ratio = float(large_side - m_margin_top  - m_margin_bottom) /
                          float(small_side - m_margin_left - m_margin_right);

            m_coord_system_width  = (int)((small_side_cm - m_margin_left/10.f -
                                           m_margin_right/10.0f)*m_units_per_cm);
            m_coord_system_height = m_coord_system_width*ratio;
        }
        else
        {
            float ratio = float(large_side - m_margin_left - m_margin_right) /
                          float(small_side - m_margin_top  - m_margin_bottom);

            m_coord_system_height = (int)((small_side_cm - m_margin_top/10.0f -
                                           m_margin_bottom/10.0f)* m_units_per_cm);
            m_coord_system_width  = m_coord_system_height*ratio;

        }

    }

    /** returns the data obtained from the page setup dialog (or the defaults,
     * if dialog was not shown) */
    wxPrintData getPrintData()
    {
        return m_page_setup.GetPrintData();
    }

    /** Called when starting to print a document */
    bool OnBeginDocument(int startPage, int endPage)
    {
        std::cout << "beginning to print document, from page " << startPage
                  << " to " << endPage << std::endl;
        return wxPrintout::OnBeginDocument(startPage, endPage);
    }

    /** wx will call this to know how many pages can be printed */
    void GetPageInfo(int *minPage, int *maxPage, int *pageSelFrom, int *pageSelTo)
    {
        *minPage = 1;
        *maxPage = m_page_amount;

        *pageSelFrom = 1;
        *pageSelTo = m_page_amount;
    }

    /** called by wx to know what pages this document has */
    bool HasPage(int pageNum)
    {
        // wx will call this to know how many pages can be printed
        return pageNum >= 1 && pageNum <= m_page_amount;
    }


    /** called by wx everytime it's time to render a specific page onto the
     * printing device context */
    bool OnPrintPage(int pageNum)
    {
        std::cout << "printing page " << pageNum << std::endl;

        // ---- setup DC with coordinate system ----
        FitThisSizeToPageMargins(wxSize(m_coord_system_width, m_coord_system_height), m_page_setup);

        wxRect fitRect = GetLogicalPageMarginsRect(m_page_setup);

        wxCoord xoff = (fitRect.width - m_coord_system_width) / 2;
        wxCoord yoff = (fitRect.height - m_coord_system_height) / 2;
        OffsetLogicalOrigin(xoff, yoff);

        wxDC* ptr = GetDC();
        if (ptr==NULL || !ptr->IsOk())
        {
            std::cout << "DC is not Ok, interrupting printing" << std::endl;
            return false;
        }
        wxDC& dc = *ptr;

        // ---- A couple helper variables to help us during draw within paper area -----
        const int x0 = 0;
        const int y0 = 0;
        const int width = m_coord_system_width;
        const int height = m_coord_system_height;
        const int x1 = x0 + width;
        const int y1 = y0 + height;

        const int center_x = x0 + width/2;
        const int center_y = y0 + height/2;

        std::cout << "printable area : (" << x0 << ", " << y0 << ") to ("
        << x1 << ", " << y1 << ")" << std::endl;

        // ---- Draw to the print DC ----
        dc.Clear();

        dc.SetPen(  wxPen( wxColour(0,0,0), brush_size ) );
        dc.SetBrush( *wxTRANSPARENT_BRUSH );

        // draw a rectangle to show its bounds.
        dc.DrawRectangle(x0, y0, width, height);

        // draw wxWidgets logo
        dc.SetBrush( *wxRED_BRUSH );
        dc.DrawRectangle(center_x-45-38, center_y, 76, 76);
        dc.SetBrush( *wxBLUE_BRUSH );
        dc.DrawRectangle(center_x-38, center_y-45, 76, 76);
        dc.SetBrush( wxBrush( wxColor(255,244,0) ) );
        dc.DrawRectangle(center_x+45-38, center_y-10, 76, 76);

        // draw page number label
        wxString label( wxT("This is page #") );
        label << pageNum;
        dc.SetTextBackground( wxColour(255,255,0) );
        dc.SetTextForeground( wxColour(0,0,0) );
        dc.DrawText( label, x0 + width/5, y0 + height - 50 );

        return true;
    }

    /** Called when printing is done. I have nothing to do in this case
     * in this example. */
    void OnEndPrinting()
    {
    }
};









int MRKprinterDialog::Print()
{

     QuickPrint*  myprint = new QuickPrint( 5 /* 5 pages */,
                                           wxT("wxPrint test"),
                                           30 /* 30 units per centimeter */ );
    if (!myprint->performPageSetup(true))
    {
        // user cancelled
        exit(0);
    }

    wxPrintDialogData data(myprint->getPrintData());
    wxPrinter printer(&data);
    const bool success = printer.Print(NULL, myprint, true /* show dialog */);

    delete myprint;

    if (!success)
    {
        std::cerr << "Failed!!\n";
        return false;
    }
    exit(0);
    return true;





}










void MRKprinterDialog::GetAllMrkFiles()
{
wxArrayString str;
   wxStandardPaths std;
    wxString exePath =wxT("/home/milind");          //= std.GetExecutablePath();


  //Get All Files in /usr/local/include  directory
  //Set wxDIR_FILES flag. There's no wxDIR_DIRS in flags argument, so
  //this command lists only files under /usr/local/include directory non-recursively.
  wxDir::GetAllFiles(exePath,&str,wxEmptyString,wxDIR_FILES);

  for (unsigned int index=0;index < str.GetCount();index++)
  {
    wxMessageBox(str[index], _("Welcome to..."));
  }

  }
