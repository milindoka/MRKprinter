

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
     QuickPrint(int page_amount, wxString title, float units_per_cm);

     /** shows the page setup dialog, OR sets up defaults */
    bool performPageSetup(const bool showPageSetupDialog);

    /**
      * @param page_amount    number of pages we want to print. Here it's static because it's just a test, but in
      *                       real code you will often want to calculate dynamically once you know the size of the
      *                       printable area from page setup
      * @param title          name of the print job / of the printed document
      * @param units_per_cem  we'll use this to determine the coordinate system; it describes the number of units
      *                       per centimeter (i.e. how fine the coordinate system is)
      */

};


