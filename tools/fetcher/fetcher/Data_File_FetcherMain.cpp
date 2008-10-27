/***************************************************************
 * Name:      Data_File_FetcherMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Ivan Vucica (ivucica@gmail.com)
 * Created:   2008-10-27
 * Copyright: Ivan Vucica (http://www.objectnetworks.net/)
 * License:
 **************************************************************/

#include "Data_File_FetcherMain.h"
#include <wx/msgdlg.h>

#include <wx/sstream.h>
#include <wx/protocol/http.h>

//(*InternalHeaders(Data_File_FetcherDialog)
#include <wx/settings.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)





// khaotic vars
bool isdownloading = false;
wxHTTP *h;
wxInputStream *h_stream;
// khaotic vars




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

//(*IdInit(Data_File_FetcherDialog)
const long Data_File_FetcherDialog::ID_STATICTEXT1 = wxNewId();
const long Data_File_FetcherDialog::ID_BUTTON1 = wxNewId();
const long Data_File_FetcherDialog::ID_STATICLINE1 = wxNewId();
const long Data_File_FetcherDialog::ID_BUTTON2 = wxNewId();
const long Data_File_FetcherDialog::ID_STATICTEXT2 = wxNewId();
const long Data_File_FetcherDialog::ID_TEXTCTRL1 = wxNewId();
const long Data_File_FetcherDialog::ID_TEXTCTRL2 = wxNewId();
const long Data_File_FetcherDialog::ID_BUTTON3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(Data_File_FetcherDialog,wxDialog)
    //(*EventTable(Data_File_FetcherDialog)
    EVT_IDLE(Data_File_FetcherDialog::OnIdle)
    //*)
END_EVENT_TABLE()

Data_File_FetcherDialog::Data_File_FetcherDialog(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(Data_File_FetcherDialog)
    wxBoxSizer* BoxSizer4;
    wxBoxSizer* BoxSizer3;

    Create(parent, id, _("wxWidgets app"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
    SetClientSize(wxSize(599,86));
    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("YATC\nData File Fetcher"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    wxFont StaticText1Font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    if ( !StaticText1Font.Ok() ) StaticText1Font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    StaticText1Font.SetPointSize(20);
    StaticText1->SetFont(StaticText1Font);
    BoxSizer3->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 10);
    BoxSizer2 = new wxBoxSizer(wxVERTICAL);
    Button1 = new wxButton(this, ID_BUTTON1, _("About"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    BoxSizer2->Add(Button1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxSize(10,-1), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
    BoxSizer2->Add(StaticLine1, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    Button2 = new wxButton(this, ID_BUTTON2, _("Quit"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    BoxSizer2->Add(Button2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    BoxSizer3->Add(BoxSizer2, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    BoxSizer1->Add(BoxSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Type in latest version:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    BoxSizer4->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl1 = new wxTextCtrl(this, ID_TEXTCTRL1, _("8"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    TextCtrl1->SetMaxLength(1);
    BoxSizer4->Add(TextCtrl1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl2 = new wxTextCtrl(this, ID_TEXTCTRL2, _("31"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    TextCtrl2->SetMaxLength(2);
    BoxSizer4->Add(TextCtrl2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button3 = new wxButton(this, ID_BUTTON3, _("Download"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    BoxSizer4->Add(Button3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(BoxSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(BoxSizer1);
    BoxSizer1->SetSizeHints(this);

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Data_File_FetcherDialog::OnAbout);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Data_File_FetcherDialog::OnQuit);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Data_File_FetcherDialog::OnButton3Click);
    //*)
}

Data_File_FetcherDialog::~Data_File_FetcherDialog()
{
    //(*Destroy(Data_File_FetcherDialog)
    //*)
}

void Data_File_FetcherDialog::OnQuit(wxCommandEvent& event)
{
    Close();
}

void Data_File_FetcherDialog::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void Data_File_FetcherDialog::OnButton3Click(wxCommandEvent& event)
{
    h = new wxHTTP();

    isdownloading=true;

}

void Data_File_FetcherDialog::OnIdle(wxIdleEvent& event)
{
    if (isdownloading){
        wxStringOutputStream sos;
        if (h_stream){

            if (h_stream->CanRead()){
                printf(".\n");
                h_stream->Read(sos);
                sos.Write(*h_stream);
            }
        } else{
            printf("no hstream\n");
            h_stream = h->GetInputStream(_("http://tornado.zrs.hr/~ivucica/yatc/!README"));
        }

        //delete h;

    }
}
