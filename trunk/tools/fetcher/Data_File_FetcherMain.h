/***************************************************************
 * Name:      Data_File_FetcherMain.h
 * Purpose:   Defines Application Frame
 * Author:    Ivan Vucica (ivucica@gmail.com)
 * Created:   2008-10-27
 * Copyright: Ivan Vucica (http://www.objectnetworks.net/)
 * License:
 **************************************************************/

#ifndef DATA_FILE_FETCHERMAIN_H
#define DATA_FILE_FETCHERMAIN_H

//(*Headers(Data_File_FetcherDialog)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/gauge.h>
//*)

class Data_File_FetcherDialog: public wxDialog
{
    public:

        Data_File_FetcherDialog(wxWindow* parent,wxWindowID id = -1);
        virtual ~Data_File_FetcherDialog();



        static const long ID_WORKEREVENT;

    private:

        //(*Handlers(Data_File_FetcherDialog)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnButton3Click(wxCommandEvent& event);
        void OnIdle(wxIdleEvent& event);
        //*)
        void OnWorkerEvent(wxCommandEvent& event);

        //(*Identifiers(Data_File_FetcherDialog)
        static const long ID_STATICTEXT1;
        static const long ID_BUTTON1;
        static const long ID_STATICLINE1;
        static const long ID_BUTTON2;
        static const long ID_STATICTEXT2;
        static const long ID_TEXTCTRL1;
        static const long ID_TEXTCTRL2;
        static const long ID_BUTTON3;
        static const long ID_GAUGE1;
        //*)

        //(*Declarations(Data_File_FetcherDialog)
        wxStaticText* StaticText2;
        wxButton* Button1;
        wxGauge* Gauge1;
        wxStaticText* StaticText1;
        wxBoxSizer* BoxSizer2;
        wxButton* Button2;
        wxButton* Button3;
        wxStaticLine* StaticLine1;
        wxTextCtrl* TextCtrl2;
        wxBoxSizer* BoxSizer1;
        wxTextCtrl* TextCtrl1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // DATA_FILE_FETCHERMAIN_H
