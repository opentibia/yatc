/***************************************************************
 * Name:      Data_File_FetcherApp.h
 * Purpose:   Defines Application Class
 * Author:    Ivan Vucica (ivucica@gmail.com)
 * Created:   2008-10-27
 * Copyright: Ivan Vucica (http://www.objectnetworks.net/)
 * License:
 **************************************************************/

#ifndef DATA_FILE_FETCHERAPP_H
#define DATA_FILE_FETCHERAPP_H

#include <wx/app.h>

class Data_File_FetcherApp : public wxApp
{
    public:
        virtual bool OnInit();
};

#endif // DATA_FILE_FETCHERAPP_H
