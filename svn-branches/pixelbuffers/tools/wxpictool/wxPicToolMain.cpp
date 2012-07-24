/***************************************************************
 * Name:      wxPicToolMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Ivan Vucica (ivucica@gmail.com)
 * Created:   2008-12-27
 * Copyright: Ivan Vucica (http://www.objectnetworks.net/)
 * License:   GNU General Public License v2
 **************************************************************/

#include "wxPicToolMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(wxPicToolFrame)
#include <wx/settings.h>
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <errno.h>
#include "../pictool/picfuncs.h"
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

//(*IdInit(wxPicToolFrame)
const long wxPicToolFrame::ID_STATICTEXT1 = wxNewId();
const long wxPicToolFrame::ID_LBLPIC = wxNewId();
const long wxPicToolFrame::ID_TXTPIC = wxNewId();
const long wxPicToolFrame::ID_LISTBOX1 = wxNewId();
const long wxPicToolFrame::ID_TXTTBCHOICE = wxNewId();
const long wxPicToolFrame::ID_STATICTEXT2 = wxNewId();
const long wxPicToolFrame::ID_BUTTON4 = wxNewId();
const long wxPicToolFrame::ID_PANEL3 = wxNewId();
const long wxPicToolFrame::ID_LISTBOX2 = wxNewId();
const long wxPicToolFrame::ID_TEXTCTRL2 = wxNewId();
const long wxPicToolFrame::ID_STATICTEXT6 = wxNewId();
const long wxPicToolFrame::ID_BUTTON5 = wxNewId();
const long wxPicToolFrame::ID_PANEL5 = wxNewId();
const long wxPicToolFrame::ID_BUTTON1 = wxNewId();
const long wxPicToolFrame::ID_STATICTEXT4 = wxNewId();
const long wxPicToolFrame::ID_PANEL1 = wxNewId();
const long wxPicToolFrame::ID_STATICTEXT5 = wxNewId();
const long wxPicToolFrame::ID_BUTTON3 = wxNewId();
const long wxPicToolFrame::ID_PANEL2 = wxNewId();
const long wxPicToolFrame::ID_NOTEBOOK1 = wxNewId();
const long wxPicToolFrame::ID_BTNPIC = wxNewId();
const long wxPicToolFrame::ID_STATICTEXT3 = wxNewId();
const long wxPicToolFrame::ID_TEXTCTRL1 = wxNewId();
const long wxPicToolFrame::ID_BUTTON2 = wxNewId();
const long wxPicToolFrame::idMenuQuit = wxNewId();
const long wxPicToolFrame::idMenuAbout = wxNewId();
const long wxPicToolFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(wxPicToolFrame,wxFrame)
    //(*EventTable(wxPicToolFrame)
    //*)
END_EVENT_TABLE()

wxPicToolFrame::wxPicToolFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(wxPicToolFrame)
    wxMenuItem* MenuItem2;
    wxMenuItem* MenuItem1;
    wxMenu* Menu1;
    wxMenuBar* MenuBar1;
    wxMenu* Menu2;

    Create(parent, wxID_ANY, _("wxPicTool"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxMINIMIZE_BOX|wxSTATIC_BORDER, _T("wxID_ANY"));
    SetClientSize(wxSize(400,405));
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
    lblProgramName = new wxStaticText(this, ID_STATICTEXT1, _("wxPicTool"), wxPoint(16,24), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    wxFont lblProgramNameFont(24,wxDEFAULT,wxFONTSTYLE_NORMAL,wxBOLD,false,_T("Arial"),wxFONTENCODING_DEFAULT);
    lblProgramName->SetFont(lblProgramNameFont);
    lblPic = new wxStaticText(this, ID_LBLPIC, _("Choose PIC:"), wxPoint(16,96), wxDefaultSize, 0, _T("ID_LBLPIC"));
    txtPic = new wxTextCtrl(this, ID_TXTPIC, _("Tibia.pic"), wxPoint(96,88), wxSize(152,21), 0, wxDefaultValidator, _T("ID_TXTPIC"));
    nbActions = new wxNotebook(this, ID_NOTEBOOK1, wxPoint(8,150), wxSize(384,208), 0, _T("ID_NOTEBOOK1"));
    Panel3 = new wxPanel(nbActions, ID_PANEL3, wxDefaultPosition, wxSize(297,182), wxTAB_TRAVERSAL, _T("ID_PANEL3"));
    lstTBChoice = new wxListBox(Panel3, ID_LISTBOX1, wxPoint(16,16), wxSize(132,120), 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX1"));
    lstTBChoice->Append(_("0 - Background"));
    lstTBChoice->Append(_("1 - Hints"));
    lstTBChoice->Append(_("2 - Font"));
    lstTBChoice->Append(_("3 - UI"));
    lstTBChoice->Append(_("4 - Outline font"));
    lstTBChoice->Append(_("5 - Minifont"));
    lstTBChoice->Append(_("6 - Lighting"));
    lstTBChoice->Append(_("7 - AAFont"));
    txtTBChoice = new wxTextCtrl(Panel3, ID_TXTTBCHOICE, wxEmptyString, wxPoint(88,144), wxSize(56,21), 0, wxDefaultValidator, _T("ID_TXTTBCHOICE"));
    StaticText1 = new wxStaticText(Panel3, ID_STATICTEXT2, _("Custom:"), wxPoint(24,152), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    btnExtractOne = new wxButton(Panel3, ID_BUTTON4, _("Extract to ..."), wxPoint(190,72), wxSize(150,23), 0, wxDefaultValidator, _T("ID_BUTTON4"));
    page = new wxPanel(nbActions, ID_PANEL5, wxDefaultPosition, wxSize(297,182), wxTAB_TRAVERSAL, _T("ID_PANEL5"));
    lstFBChoice = new wxListBox(page, ID_LISTBOX2, wxPoint(16,16), wxSize(132,120), 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX2"));
    lstFBChoice->Append(_("0 - Background"));
    lstFBChoice->Append(_("1 - Hints"));
    lstFBChoice->Append(_("2 - Font"));
    lstFBChoice->Append(_("3 - UI"));
    lstFBChoice->Append(_("4 - Outline font"));
    lstFBChoice->Append(_("5 - Minifont"));
    lstFBChoice->Append(_("6 - Lighting"));
    lstFBChoice->Append(_("7 - AAFont"));
    txtFBChoice = new wxTextCtrl(page, ID_TEXTCTRL2, wxEmptyString, wxPoint(88,144), wxSize(56,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    StaticText5 = new wxStaticText(page, ID_STATICTEXT6, _("Custom:"), wxPoint(24,152), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    btnImportOne = new wxButton(page, ID_BUTTON5, _("Import from ..."), wxPoint(190,72), wxSize(150,23), 0, wxDefaultValidator, _T("ID_BUTTON5"));
    Panel1 = new wxPanel(nbActions, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    btnExtractAll = new wxButton(Panel1, ID_BUTTON1, _("Extract all"), wxPoint(144,80), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    StaticText3 = new wxStaticText(Panel1, ID_STATICTEXT4, _("Files will be called Tibia0.bmp, Tibia1.bmp, ..."), wxPoint(80,48), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    Panel2 = new wxPanel(nbActions, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    StaticText4 = new wxStaticText(Panel2, ID_STATICTEXT5, _("Pictures will be read from Tibia0.bmp, Tibia1.bmp, ..."), wxPoint(72,48), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    btnImportAll = new wxButton(Panel2, ID_BUTTON3, _("Import all"), wxPoint(144,80), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    nbActions->AddPage(Panel3, _("One to BMP"), false);
    nbActions->AddPage(page, _("One from BMP"), false);
    nbActions->AddPage(Panel1, _("All to BMP"), false);
    nbActions->AddPage(Panel2, _("All from BMP"), false);
    btnPic = new wxButton(this, ID_BTNPIC, _("Browse..."), wxPoint(264,88), wxSize(85,24), 0, wxDefaultValidator, _T("ID_BTNPIC"));
    StaticText2 = new wxStaticText(this, ID_STATICTEXT3, _("Choose dir:"), wxPoint(16,120), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    txtOutput = new wxTextCtrl(this, ID_TEXTCTRL1, _("./"), wxPoint(96,112), wxSize(152,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    btnOutput = new wxButton(this, ID_BUTTON2, _("Browse..."), wxPoint(264,112), wxSize(85,24), 0, wxDefaultValidator, _T("ID_BUTTON2"));
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);

    Connect(ID_LISTBOX1,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&wxPicToolFrame::OnlstTBChoiceSelect);
    Connect(ID_TXTTBCHOICE,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&wxPicToolFrame::OntxtTBChoiceText);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxPicToolFrame::OnbtnExtractOneClick);
    Connect(ID_LISTBOX2,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&wxPicToolFrame::OnlstFBChoiceSelect);
    Connect(ID_TEXTCTRL2,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&wxPicToolFrame::OntxtFBChoiceText);
    Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxPicToolFrame::OnbtnImportOneClick);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxPicToolFrame::OnbtnExtractAllClick);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxPicToolFrame::OnbtnImportAllClick);
    Connect(ID_BTNPIC,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxPicToolFrame::OnbtnPicClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxPicToolFrame::OnbtnOutputClick);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxPicToolFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxPicToolFrame::OnAbout);
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&wxPicToolFrame::OnClose);
    //*)
}

wxPicToolFrame::~wxPicToolFrame()
{
    //(*Destroy(wxPicToolFrame)
    //*)
}

void wxPicToolFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void wxPicToolFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(_("wxPicTool 0.8 SVN\n"
                   "\n"
                   "Adaptation of ANSI C PicTool from OpenTibia project.\n"
                   "ANSI C PicTool (c) 2007-2008 Ivan Vucica\n"
                   "wxPicTool (c) 2008 Ivan Vucica\n"
                   "\n"
                   "wxPicTool comes with ABSOLUTELY NO WARRANTY; \n"
                   "for details see sections 11 and 12 in file COPYING.\n"
                   "This is free software, and you are welcome \n"
		           "to redistribute it under certain conditions;\n"
		           "see file COPYING for details.\n"
		           "\n")
                   + msg, _("wxPicTool"));
}

void wxPicToolFrame::OnClose(wxCloseEvent& event)
{
    Close();
}


void wxPicToolFrame::OnbtnPicClick(wxCommandEvent& event)
{
    wxString fn = wxLoadFileSelector("", "pic");
    if (!fn.size())
        return;
    txtPic->SetValue(fn);
}

void wxPicToolFrame::OnbtnOutputClick(wxCommandEvent& event)
{
    wxString fn = wxDirSelector();
    if (!fn.size())
        return;
    txtOutput->SetValue(fn);
}



int indexchoice_tb=-1;
int indexchoice_fb=-1;


//////////////////////////
void wxPicToolFrame::OnlstTBChoiceSelect(wxCommandEvent& event)
{
    wxString s;
    s.Printf("%d", lstTBChoice->GetSelection());
    txtTBChoice->SetValue(s);
}
void wxPicToolFrame::OnlstFBChoiceSelect(wxCommandEvent& event)
{
    wxString s;
    s.Printf("%d", lstFBChoice->GetSelection());
    txtFBChoice->SetValue(s);
}
////////////////////////////
void wxPicToolFrame::OntxtTBChoiceText(wxCommandEvent& event)
{
    wxString s;
    indexchoice_tb = atoi(txtTBChoice->GetValue().c_str());
    s.Printf("Extract to Tibia%d.bmp", indexchoice_tb);
    btnExtractOne->SetLabel(s);
}


void wxPicToolFrame::OntxtFBChoiceText(wxCommandEvent& event)
{
    wxString s;
    indexchoice_fb = atoi(txtFBChoice->GetValue().c_str());
    s.Printf("Import from Tibia%d.bmp", indexchoice_fb);
    btnImportOne->SetLabel(s);
}
////////////////////////////////
void wxPicToolFrame::OnbtnExtractAllClick(wxCommandEvent& event)
{
    wxMessageBox(_("Functionality not done yet."), _("TODO"));
}

void wxPicToolFrame::OnbtnImportAllClick(wxCommandEvent& event)
{
    wxMessageBox(_("Functionality not done yet."), _("TODO"));
}
/////////////////////////////////
void wxPicToolFrame::OnbtnExtractOneClick(wxCommandEvent& event)
{

    if (indexchoice_tb == -1) {
        wxMessageBox(_("Make a choice first, then give commands, sir."), _("I'm sorry, captain, I can't do much more"));
        return;
    }
    SDL_Surface* s;

    SDL_Init(SDL_INIT_VIDEO);
    wxString fn;
    fn.Printf("Tibia%d.bmp", indexchoice_tb);
    fn = txtOutput->GetValue() + "/" + fn;

    if(readpic(txtPic->GetValue().c_str(), indexchoice_tb, &s)){
        wxMessageBox(_("Reading of the sprite file has failed.\n"
                       "Hast thou messed it up?\n"
                       "Hast thou placed it in wrong place?\n"
                       "Thee must fix the file."), _("I'm sorry, captain, I can't do much more"));
        SDL_Quit();
        return;
    }

    if(SDL_SaveBMP(s, fn.c_str())){
        wxMessageBox(_("Thy bitmap file is unwritable"), _("I'm sorry, captain, I can't do much more"));
        SDL_FreeSurface(s);
        SDL_Quit();
        return;
    }
    wxMessageBox(_("Thou hast succeeded, mighty nightly lord!"), _("YOU WIN"));

    SDL_FreeSurface(s);
    SDL_Quit();


}

void wxPicToolFrame::OnbtnImportOneClick(wxCommandEvent& event)
{
    if (indexchoice_fb == -1) {
        wxMessageBox(_("Make a choice first, then give commands, sir."), _("I'm sorry, captain, I can't do much more"));
        return;
    }

    SDL_Init(SDL_INIT_VIDEO);

    wxString fn;
    fn.Printf("Tibia%d.bmp", indexchoice_fb);
    fn = txtOutput->GetValue() + "/" + fn;

    SDL_Surface* s = SDL_LoadBMP(fn.c_str());
    if (!s) {
        wxString msg;
        msg = "It would be prudent to first make sure that " + fn + " exists, Master.";
        wxMessageBox(msg, _("I'm sorry, captain, I can't do much more"));

        SDL_Quit();
        return;
    }
    int errorid;
    if (errorid = writepic(txtPic->GetValue().c_str(), indexchoice_fb, s))
    {
        switch(errorid){
        default: {
            wxString msg;
            msg.Printf((_(
                        "Apparently, there is an issue with reading or writing ") + txtPic->GetValue() + ".\n"
                        "\n"
                        "Errid: %d\n"
                        "Errno: %d\n"
                        "Strerror: %s" ).c_str(), errorid, errno, strerror(errno));
            wxMessageBox(msg, _("I'm sorry, captain, I can't do much more"));

            SDL_Quit();
            return;
        }
        case -1: {
            wxString msg;
            msg.Printf((_(
                        "File ") + txtPic->GetValue() + " does not exist.\n"
                        "\n"
                        "Errid: %d\n"
                        "Errno: %d\n"
                        "Strerror: %s" ).c_str(), errorid, errno, strerror(errno));
            wxMessageBox(msg, _("I'm sorry, captain, I can't do much more"));
            SDL_Quit();
            return;
        }
        case 92:   // rename() failed
        case 93: { // rename's unlink() failed

            wxString msg;
            msg.Printf("Rename from __tmp__.pic failed.\n"
                       "File has been outputted successfully into __tmp__pic, but you'll have to pick it up yourself.\n"
                       "\n"
                       "Errid: %d\n"
                       "Errno: %d\n"
                       "Strerror: %s", errorid, errno, strerror(errno));
            wxMessageBox(msg, _("Aff"));
            SDL_Quit();
            break;
        }
        }
    }

    wxMessageBox(_("Thou hast succeeded, mighty nightly lord!"), _("YOU WIN"));

    SDL_FreeSurface(s);
    SDL_Quit();
}
