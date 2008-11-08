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
        void OnBtnDownloadClick(wxCommandEvent& event);
        void OnIdle(wxIdleEvent& event);
        //*)
        void OnWorkerEvent(wxCommandEvent& event);

        //(*Identifiers(Data_File_FetcherDialog)
        static const long ID_STATICTEXT1;
        static const long ID_BTNABOUT;
        static const long ID_STATICLINE1;
        static const long ID_BTNQUIT;
        static const long ID_STATICTEXT3;
        static const long ID_GAUGE1;
        static const long ID_STATICTEXT2;
        static const long ID_TEXTCTRL1;
        static const long ID_TEXTCTRL2;
        static const long ID_BTNDOWNLOAD;
        //*)

        //(*Declarations(Data_File_FetcherDialog)
        wxStaticText* LblStatus;
        wxStaticText* LblTypeIn;
        wxButton* BtnAbout;
        wxStaticText* StaticText1;
        wxBoxSizer* BoxSizer2;
        wxTextCtrl* TxtVerMinor;
        wxGauge* GauProgress;
        wxStaticLine* StaticLine1;
        wxBoxSizer* BoxSizer1;
        wxButton* BtnQuit;
        wxTextCtrl* TxtVerMajor;
        wxButton* BtnDownload;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // DATA_FILE_FETCHERMAIN_H
