/***************************************************************
 * Name:      wxPicToolApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Ivan Vucica (ivucica@gmail.com)
 * Created:   2008-12-27
 * Copyright: Ivan Vucica (http://www.objectnetworks.net/)
 * License:   GNU General Public License v2
 **************************************************************/

#include "wxPicToolApp.h"

//(*AppHeaders
#include "wxPicToolMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(wxPicToolApp);

bool wxPicToolApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	wxPicToolFrame* Frame = new wxPicToolFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
