/***************************************************************
 * Name:      Data_File_FetcherMain.cpp
 * Purpose:   Code for Application Frame; Fetching Code
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
#include <wx/stopwatch.h>
#include <wx/zstream.h>
#include <wx/tarstrm.h>

#include <sstream>
#include <iomanip>
#include <memory>

//(*InternalHeaders(Data_File_FetcherDialog)
#include <wx/settings.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)




// for stat
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
// end for stat


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
const long Data_File_FetcherDialog::ID_BTNABOUT = wxNewId();
const long Data_File_FetcherDialog::ID_STATICLINE1 = wxNewId();
const long Data_File_FetcherDialog::ID_BTNQUIT = wxNewId();
const long Data_File_FetcherDialog::ID_STATICTEXT3 = wxNewId();
const long Data_File_FetcherDialog::ID_GAUGE1 = wxNewId();
const long Data_File_FetcherDialog::ID_STATICTEXT2 = wxNewId();
const long Data_File_FetcherDialog::ID_TEXTCTRL1 = wxNewId();
const long Data_File_FetcherDialog::ID_TEXTCTRL2 = wxNewId();
const long Data_File_FetcherDialog::ID_BTNDOWNLOAD = wxNewId();
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
wxString version;

class FetchThread : public wxThread
{
public:
    FetchThread(Data_File_FetcherDialog *frame) : wxThread() {
        m_frame = frame;
        h = new wxHTTP();
    }

    ExitCode Entry() {
        isdownloading = true;

        /*
        if (!unpackNow()) {
            castEvent(13);
            wxThread::Sleep(100);
            isdownloading = false;
        } else {
            castEvent(1);
            wxThread::Sleep(100);
            isdownloading = false;
        }
        return 0;
        */


        count = 0;
        //h->SetHeader ( wxT("Accept") , wxT("text/*") );
        //h->SetHeader ( wxT("User-Agent"), wxGetApp().GetAppName() );
        h->SetTimeout ( 120 );

        if (!h->Connect(_("download.tibia.com"),80)) {
            castEvent(2);
            isdownloading = false;
            wxThread::Sleep(100);
            return 0;
        }

        castEvent(7);


        h_stream = h->GetInputStream(_("/") + version + ".tgz");
        if (!h_stream) {
            printf("Failed to send request\n");
            castEvent(2);
            isdownloading = false;
            wxThread::Sleep(100);
            return 0;
        }


        castEvent(8);

        {
            wxCriticalSectionLocker locker(m_critsect);

            wxString some_wxstring = h->GetHeader("Content-length");
            contentlength = atoi(some_wxstring.c_str());
            printf("Length: %d\n", contentlength);
        }

        struct stat st;
        stat((version + ".tgz").c_str(), &st);
        size_t size = st.st_size;
        if (size != contentlength)
        {

            unlink((version + ".tgz").c_str());

            wxFileOutputStream fos(version + ".tgz");
            if (!fos.IsOk()){
                castEvent(3);
                isdownloading = false;
                wxThread::Sleep(100);
                return 0;
            }
            wxMilliClock_t lasttime = 0;
            do {
                char buf[1024];
                if (TestDestroy()) {
                    castEvent(4);
                    isdownloading = false;
                    wxThread::Sleep(100);
                    return 0;
                }

                h_stream->Read(buf, sizeof(buf));
                fos.Write(buf, h_stream->LastRead());

                increaseCount(h_stream->LastRead());

                if (wxGetLocalTimeMillis() - lasttime >= 50) {
                    lasttime = wxGetLocalTimeMillis();
                    castEvent(5);
                }

            } while(h_stream->LastRead());


            delete h_stream; h_stream = NULL;
            delete h; h = NULL;
        }

        if (!unpackNow()) {
            castEvent(13);
            wxThread::Sleep(100);
            isdownloading = false;
        } else {
            castEvent(1);
            wxThread::Sleep(100);
            isdownloading = false;
        }


        return 0;
    }

    void increaseCount(int amount){
        wxCriticalSectionLocker locker(m_critsect);
        count += amount;
    }

    bool unpackNow() {
        wxFFileInputStream fis(version + ".tgz");
        wxZlibInputStream zlis(fis);
        wxTarInputStream tis(zlis);

        if (!tis.CanRead()) {
            return false;
        }

        std::auto_ptr<wxTarEntry> entry;
        while (entry.reset(tis.GetNextEntry()), entry.get() != NULL) {
            // access meta-data
            wxString name = entry->GetName();
            printf("%s\n", name.c_str());
            if (name == "Tibia/Tibia.pic" || name == "Tibia\\Tibia.pic") {
                castEvent(10);
                unpackEntry(tis, "Tibia.pic");
            } else if (name == "Tibia/Tibia.dat" || name == "Tibia\\Tibia.dat") {
                castEvent(11);
                unpackEntry(tis, "Tibia.dat");
            } else if (name == "Tibia/Tibia.spr" || name == "Tibia\\Tibia.spr") {
                castEvent(12);
                unpackEntry(tis, "Tibia.spr");
            } else {
                printf("Name unrecognized\n");
                castEvent(9);
            }
        }
        return true;
    }
    void unpackEntry(wxTarInputStream& tis, const char* fn) {
        wxFileOutputStream fos(fn);
        fos.Write(tis);
    }


    void castEvent(int type) {
        // 1 == done
        // 2 == failed to connect
        // 3 == failed to open output file
        // 4 == terminated by main loop
        // 5 == update progress bar
        // 6 == unexpected http response

        // 7 == connected, waiting for response
        // 8 == downloading

        // 9 == seeking in tar
        // 10 == unpacking tibia.pic
        // 11 == unpacking tibia.dat
        // 12 == unpacking tibia.spr

        // 13 == unpack failed
        wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, Data_File_FetcherDialog::ID_WORKEREVENT );
        event.SetInt( type );

        // send in a thread-safe way
        wxPostEvent( m_frame, event );

    }

    float getProgress() {
        wxCriticalSectionLocker locker(m_critsect);
        return _getProgress();
    }
    float _getProgress() {
        if (!contentlength) return 0;
        return (((float)count) / contentlength)*100;

    }
    std::string getOutputString() {
        wxCriticalSectionLocker locker(m_critsect);
        std::stringstream s;

        s << std::setprecision(4) << _getProgress() << "% - ";
        if (count < 1024)
            s << count << "B";
        else if (count < 1024*1024)
            s << count/1024. << "KiB";
        else
            s << count/(1024.*1024.) << "MiB";
        s << " of ";
        s << contentlength / (1024*1024.) << "MiB";

//        printf("S: %s\n", s.str().c_str());
        return s.str();
    }

    wxCriticalSection m_critsect;

private:

    wxHTTP *h;
    wxInputStream *h_stream;
    Data_File_FetcherDialog *m_frame;
    int contentlength, count;
    bool pickedUpStatus;

} * fetchThread;

// end khaotic vars




Data_File_FetcherDialog::Data_File_FetcherDialog(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(Data_File_FetcherDialog)
    wxFlexGridSizer* FlexGridSizer2;
    wxFlexGridSizer* FlexGridSizer1;
    wxBoxSizer* BoxSizer3;
    
    Create(parent, wxID_ANY, _("Tibia Data File Fetcher"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(599,86));
    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Tibia\nData File Fetcher"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    wxFont StaticText1Font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    if ( !StaticText1Font.Ok() ) StaticText1Font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    StaticText1Font.SetPointSize(20);
    StaticText1->SetFont(StaticText1Font);
    BoxSizer3->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 10);
    BoxSizer2 = new wxBoxSizer(wxVERTICAL);
    BtnAbout = new wxButton(this, ID_BTNABOUT, _("About"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BTNABOUT"));
    BoxSizer2->Add(BtnAbout, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    StaticLine1 = new wxStaticLine(this, ID_STATICLINE1, wxDefaultPosition, wxSize(245,2), wxLI_HORIZONTAL, _T("ID_STATICLINE1"));
    BoxSizer2->Add(StaticLine1, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    BtnQuit = new wxButton(this, ID_BTNQUIT, _("Quit"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BTNQUIT"));
    BoxSizer2->Add(BtnQuit, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    BoxSizer3->Add(BoxSizer2, 0, wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 4);
    BoxSizer1->Add(BoxSizer3, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1 = new wxFlexGridSizer(0, 1, 0, 0);
    LblStatus = new wxStaticText(this, ID_STATICTEXT3, _("Ready"), wxDefaultPosition, wxSize(536,13), wxALIGN_CENTRE, _T("ID_STATICTEXT3"));
    FlexGridSizer1->Add(LblStatus, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GauProgress = new wxGauge(this, ID_GAUGE1, 100, wxDefaultPosition, wxDLG_UNIT(this,wxSize(302,15)), 0, wxDefaultValidator, _T("ID_GAUGE1"));
    FlexGridSizer1->Add(GauProgress, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer2 = new wxFlexGridSizer(0, 4, 0, 0);
    LblTypeIn = new wxStaticText(this, ID_STATICTEXT2, _("Type in latest version:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    FlexGridSizer2->Add(LblTypeIn, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TxtVerMajor = new wxTextCtrl(this, ID_TEXTCTRL1, _("8"), wxDefaultPosition, wxSize(18,23), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    TxtVerMajor->SetMaxLength(1);
    FlexGridSizer2->Add(TxtVerMajor, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TxtVerMinor = new wxTextCtrl(this, ID_TEXTCTRL2, _("41"), wxDefaultPosition, wxSize(29,23), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    TxtVerMinor->SetMaxLength(2);
    FlexGridSizer2->Add(TxtVerMinor, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BtnDownload = new wxButton(this, ID_BTNDOWNLOAD, _("Download"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BTNDOWNLOAD"));
    FlexGridSizer2->Add(BtnDownload, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    FlexGridSizer1->Add(FlexGridSizer2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(FlexGridSizer1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(BoxSizer1);
    BoxSizer1->SetSizeHints(this);
    
    Connect(ID_BTNABOUT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Data_File_FetcherDialog::OnAbout);
    Connect(ID_BTNQUIT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Data_File_FetcherDialog::OnQuit);
    Connect(ID_BTNDOWNLOAD,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&Data_File_FetcherDialog::OnBtnDownloadClick);
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
    wxString msg = _(
        "Tibia Data File Fetcher\n"
        "\n"
        "Originally designed for YATC.\n"
        "This program will fetch the GNU/Linux version of Tibia\n"
        "and extract data files from it. Use at your own\n"
        "responsibility.\n"
        "\n"
        "Copyright (c) 2008 Ivan Vucica\n"
        "\n"
        "Tibia File Fetcher comes with ABSOLUTELY NO WARRANTY; \n"
        "for details see sections 11 and 12 in COPYING.\n"
		"This is free software, and you are welcome \n"
		"to redistribute it under certain conditions;\n"
		"see COPYING for details.\n"
		"\n"
		"If you have not received a copy of the license,\n"
		"please write to the author at: ivucica@gmail.com\n"
		"\n"
		"Do not forget! If you modify the sources, according to\n"
		"our license, you MUST share a copy of modified sources\n"
		"along with the binary.")
		    + wxbuildinfo(long_f);
    wxMessageBox(msg, _("About TDFF"));
}

void Data_File_FetcherDialog::OnBtnDownloadClick(wxCommandEvent& event)
{
    version = _("tibia") + TxtVerMajor->GetValue() + TxtVerMinor->GetValue();
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
    /*printf("Received a message\n");
    printf("%d\n", event.GetInt());*/
    switch (event.GetInt()) {

        default:
        printf("Unhandled event\n");
        break;


        case 1:// == done
        GauProgress->SetValue(100);
        LblStatus->SetLabel("Done");
        break;
        case 2:// == failed to connect
        LblStatus->SetLabel("Failed to connect\n");
        break;
        case 3:// == failed to open output file
        LblStatus->SetLabel("Failed to output file");
        break;
        case 4:// == terminated by main loop
        LblStatus->SetLabel("Terminated by main loop");
        break;
        case 5:{// == update progress bar
            std::stringstream s;
            GauProgress->SetValue(fetchThread->getProgress());

            s << "Downloading... " << fetchThread->getOutputString();
            LblStatus->SetLabel(s.str());

            break;
        }
        case 6:// == unexpected http response
        LblStatus->SetLabel("Unexpected HTTP response");
        break;
        case 7:// == connected
        LblStatus->SetLabel("Connected, waiting for response...");
        break;
        case 8:// == downloading
        LblStatus->SetLabel("Download starting...");
        break;

        case 9:// == seeking in tar
        LblStatus->SetLabel("Seeking...");
        break;
        case 10:// == tibia.pic
        LblStatus->SetLabel("Unpacking Tibia.pic...");
        GauProgress->SetValue(30);
        break;
        case 11:// == tibia.dat
        LblStatus->SetLabel("Unpacking Tibia.dat...");
        GauProgress->SetValue(60);
        break;
        case 12:// == tibia.spr
        LblStatus->SetLabel("Unpacking Tibia.spr... (this may take a while, be patient!)");
        GauProgress->SetValue(90);
        break;
        case 13:// == unpack failed
        LblStatus->SetLabel("Unpack failed");
        break;
    }
}
