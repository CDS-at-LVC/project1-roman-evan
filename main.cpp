#include <wx/wx.h>
#include <wx/filepicker.h>
#include <wx/process.h>
#include <wx/wfstream.h> 
#include <cstdio>
#include <iostream>
#include <fstream>
#include <wx/txtstrm.h>
#include <sstream>
#include <algorithm>
#include "Frames/LoginFrame.h"



class MyApp : public wxApp
{
public:
	virtual bool OnInit()
	{
		wxFrame* frame = new LoginFrame(wxT("Login"));
		frame->Show(true);
		return true;
	}
};

wxIMPLEMENT_APP(MyApp);
