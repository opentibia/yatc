/***************************************************************
 * Name:      wxPicToolMain.h
 * Purpose:   Defines Application Frame
 * Author:    Ivan Vucica (ivucica@gmail.com)
 * Created:   2008-12-27
 * Copyright: Ivan Vucica (http://www.objectnetworks.net/)
 * License:
 **************************************************************/

#ifndef WXPICTOOLMAIN_H
#define WXPICTOOLMAIN_H

//(*Headers(wxPicToolFrame)
#include <wx/notebook.h>
#include <wx/stattext.h>
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/listbox.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)

class wxPicToolFrame: public wxFrame
{
    public:

        wxPicToolFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~wxPicToolFrame();

    private:

        //(*Handlers(wxPicToolFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnChoicebook1PageChanged(wxNotebookEvent& event);
        void OnNotebook1PageChanged(wxNotebookEvent& event);
        void OnbtnPicClick(wxCommandEvent& event);
        void OnlstTBChoiceSelect(wxCommandEvent& event);
        void OntxtTBChoiceText(wxCommandEvent& event);
        void OnlstFBChoiceSelect(wxCommandEvent& event);
        void OntxtFBChoiceText(wxCommandEvent& event);
        void OnbtnExtractAllClick(wxCommandEvent& event);
        void OnbtnImportAllClick(wxCommandEvent& event);
        void OnbtnExtractOneClick(wxCommandEvent& event);
        void OnbtnImportOneClick(wxCommandEvent& event);
        void OnbtnOutputClick(wxCommandEvent& event);
        //*)

        //(*Identifiers(wxPicToolFrame)
        static const long ID_STATICTEXT1;
        static const long ID_LBLPIC;
        static const long ID_TXTPIC;
        static const long ID_LISTBOX1;
        static const long ID_TXTTBCHOICE;
        static const long ID_STATICTEXT2;
        static const long ID_BUTTON4;
        static const long ID_PANEL3;
        static const long ID_LISTBOX2;
        static const long ID_TEXTCTRL2;
        static const long ID_STATICTEXT6;
        static const long ID_BUTTON5;
        static const long ID_PANEL5;
        static const long ID_BUTTON1;
        static const long ID_STATICTEXT4;
        static const long ID_PANEL1;
        static const long ID_STATICTEXT5;
        static const long ID_BUTTON3;
        static const long ID_PANEL2;
        static const long ID_NOTEBOOK1;
        static const long ID_BTNPIC;
        static const long ID_STATICTEXT3;
        static const long ID_TEXTCTRL1;
        static const long ID_BUTTON2;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(wxPicToolFrame)
        wxListBox* lstTBChoice;
        wxTextCtrl* txtOutput;
        wxStaticText* StaticText2;
        wxButton* btnPic;
        wxButton* btnImportAll;
        wxStaticText* lblPic;
        wxStaticText* lblProgramName;
        wxPanel* Panel1;
        wxButton* btnImportOne;
        wxStaticText* StaticText1;
        wxStaticText* StaticText3;
        wxPanel* Panel3;
        wxTextCtrl* txtTBChoice;
        wxStaticText* StaticText5;
        wxButton* btnOutput;
        wxListBox* lstFBChoice;
        wxStatusBar* StatusBar1;
        wxTextCtrl* txtFBChoice;
        wxButton* btnExtractOne;
        wxPanel* Panel2;
        wxTextCtrl* txtPic;
        wxStaticText* StaticText4;
        wxPanel* page;
        wxButton* btnExtractAll;
        wxNotebook* nbActions;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // WXPICTOOLMAIN_H
