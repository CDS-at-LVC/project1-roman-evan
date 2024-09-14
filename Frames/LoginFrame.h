#pragma once

#include <wx/wx.h>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

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