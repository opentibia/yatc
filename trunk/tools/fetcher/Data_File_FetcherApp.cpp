/***************************************************************
 * Name:      Data_File_FetcherApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Ivan Vucica (ivucica@gmail.com)
 * Created:   2008-10-27
 * Copyright: Ivan Vucica (http://www.objectnetworks.net/)
 * License:   GNU General Public License v2
 **************************************************************/

#include "Data_File_FetcherApp.h"

//(*AppHeaders
#include "Data_File_FetcherMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(Data_File_FetcherApp);

bool Data_File_FetcherApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	Data_File_FetcherDialog Dlg(0);
    	SetTopWindow(&Dlg);
    	Dlg.ShowModal();
    	wxsOK = false;
    }
    //*)
    return wxsOK;

}
