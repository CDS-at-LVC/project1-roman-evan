#ifndef LOGINFRAME_H
#define LOGINFRAME_H

#include <wx/wx.h>

class LoginFrame : public wxFrame
{
public:
    LoginFrame(const wxString& title);

private:
    wxTextCtrl* userTextCtrl; // To hold username input
    wxTextCtrl* passTextCtrl; // To hold password input

    void OnLogin(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
};

#endif // LOGINFRAME_H
