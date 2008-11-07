/***************************************************************
 * Name:      Data_File_FetcherMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Ivan Vucica (ivucica@gmail.com)
 * Created:   2008-10-27
 * Copyright: Ivan Vucica (http://www.objectnetworks.net/)
 * License:   GNU General Public License v2
 **************************************************************/

#include "Data_File_FetcherMain.h"
#include <wx/msgdlg.h>

#include <wx/sstream.h>
#include <wx/protocol/http.h>
#include <wx/thread.h>
#include <wx/wfstream.h>

//(*InternalHeaders(Data_File_FetcherDialog)
#include <wx/settings.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)







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
const long Data_File_FetcherDialog::ID_GAUGE1 = wxNewId();
//*)
const long Data_File_FetcherDialog::ID_WORKEREVENT = wxNewId();

BEGIN_EVENT_TABLE(Data_File_FetcherDialog,wxDialog)
    //(*EventTable(Data_File_FetcherDialog)
    EVT_IDLE(Data_File_FetcherDialog::OnIdle)
    //*)
    EVT_MENU(ID_WORKEREVENT, Data_File_FetcherDialog::OnWorkerEvent)
END_EVENT_TABLE()




// khaotic vars
bool isdownloading = false;

class FetchThread : public wxThread
{
public:
    FetchThread(Data_File_FetcherDialog *frame) : wxThread() {
        m_frame = frame;
        h = new wxHTTP();
    }

    ExitCode Entry() {
        printf("Usao u dretvu\n");
        isdownloading = true;

        if (!h->Connect(_("download.tibia.com"))) {
            castEvent(2);
            isdownloading = false;
            wxThread::Sleep(100);
            return 0;
        }

        h_stream = h->GetInputStream("/tibia831.tgz");

        wxFileOutputStream fos("tibia831.tgz");
        if (!fos.IsOk()){
            castEvent(3);
            isdownloading = false;
            wxThread::Sleep(100);
            return 0;
        }
        do {
            char buf[1024];
            if (TestDestroy()) {
                castEvent(4);
                isdownloading = false;
                wxThread::Sleep(100);
                return 0;
            }
            printf(",");fflush(stdout);
            h_stream->Read(buf, sizeof(buf));
            printf(".");fflush(stdout);
            fos.Write(buf, h_stream->LastRead());
        } while(h_stream->LastRead());

        castEvent(1);
        wxThread::Sleep(100);
        isdownloading = false;
        return 0;
    }

    int whichEvent() const { return eventtype;}

    void castEvent(int type) {
        // 1 == done
        // 2 == failed to connect
        // 3 == failed to open output file
        // 4 == terminated by main loop
        wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, Data_File_FetcherDialog::ID_WORKEREVENT );
        event.SetInt( type );

        // send in a thread-safe way
        wxPostEvent( m_frame, event );

    }
private:

    wxHTTP *h;
    wxInputStream *h_stream;
    Data_File_FetcherDialog *m_frame;
    wxCriticalSection m_critsect;

    int eventtype;

} * fetchThread;

// end khaotic vars




Data_File_FetcherDialog::Data_File_FetcherDialog(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(Data_File_FetcherDialog)
    wxBoxSizer* BoxSizer4;
    wxBoxSizer* BoxSizer5;
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
    BoxSizer2->Add(Button1, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxSize(245,2), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
    BoxSizer2->Add(StaticLine1, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    Button2 = new wxButton(this, ID_BUTTON2, _("Quit"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    BoxSizer2->Add(Button2, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    BoxSizer3->Add(BoxSizer2, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    BoxSizer1->Add(BoxSizer3, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer4 = new wxBoxSizer(wxVERTICAL);
    BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Type in latest version:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    BoxSizer5->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl1 = new wxTextCtrl(this, ID_TEXTCTRL1, _("8"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    TextCtrl1->SetMaxLength(1);
    BoxSizer5->Add(TextCtrl1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl2 = new wxTextCtrl(this, ID_TEXTCTRL2, _("31"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    TextCtrl2->SetMaxLength(2);
    BoxSizer5->Add(TextCtrl2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button3 = new wxButton(this, ID_BUTTON3, _("Download"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    BoxSizer5->Add(Button3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer4->Add(BoxSizer5, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Gauge1 = new wxGauge(this, ID_GAUGE1, 100, wxDefaultPosition, wxDLG_UNIT(this,wxSize(303,27)), 0, wxDefaultValidator, _T("ID_GAUGE1"));
    BoxSizer4->Add(Gauge1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
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
    if (isdownloading)
        fetchThread->Delete();
    Close();
}

void Data_File_FetcherDialog::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void Data_File_FetcherDialog::OnButton3Click(wxCommandEvent& event)
{
    fetchThread = new FetchThread(this);
    fetchThread->Create();
    fetchThread->Run();
    printf("Pokrenuta dretva\n");
}

void Data_File_FetcherDialog::OnIdle(wxIdleEvent& event)
{
    /*
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
    */
}

void Data_File_FetcherDialog::OnWorkerEvent(wxCommandEvent& event){
    printf("Received a message\n");
    printf("%d\n", event.GetInt());
}
