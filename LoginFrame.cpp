#include "LoginFrame.h"
#include "AdminFrame.h"
#include "InstructorFrame.h"
#include <wx/wx.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>

using json = nlohmann::json;

LoginFrame::LoginFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(300, 200))
{
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    wxStaticText* userLabel = new wxStaticText(panel, wxID_ANY, "Username:", wxPoint(20, 20));
    userTextCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(100, 20), wxSize(150, 25));

    wxStaticText* passLabel = new wxStaticText(panel, wxID_ANY, "Password:", wxPoint(20, 60));
    passTextCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(100, 60), wxSize(150, 25), wxTE_PASSWORD);

    wxButton* loginBtn = new wxButton(panel, wxID_ANY, "Login", wxPoint(60, 110));
    wxButton* cancelBtn = new wxButton(panel, wxID_ANY, "Cancel", wxPoint(150, 110));

    loginBtn->Bind(wxEVT_BUTTON, &LoginFrame::OnLogin, this);
    cancelBtn->Bind(wxEVT_BUTTON, &LoginFrame::OnCancel, this);

    Centre();  // Center the frame on the screen
}

void LoginFrame::OnLogin(wxCommandEvent& event)
{
    wxString username = userTextCtrl->GetValue();
    wxString password = passTextCtrl->GetValue();

    if (username.IsEmpty() || password.IsEmpty())
    {
        wxMessageBox("Please enter both username and password", "Error", wxOK | wxICON_ERROR);
        return;
    }

    std::ifstream file("users-schema.json");
    if (!file.is_open()) {
        wxMessageBox("Failed to open users-schema.json", "Error", wxOK | wxICON_ERROR);
        return;
    }

    json users;
    file >> users;
    file.close();

    for (const auto& user : users)
    {
        if (user["username"] == username.ToStdString() && user["password"] == password.ToStdString())
        {
            std::string role = user["role"];
            User current_user(username.ToStdString(), password.ToStdString(), role);

            Close(true);  // Close the login window

            if (role == "admin")
            {
                AdminFrame* adminFrame = new AdminFrame("Admin Dashboard", current_user);
                adminFrame->Show(true);
            }
            else if (role == "instructor")
            {
                InstructorFrame* instructorFrame = new InstructorFrame("Instructor Dashboard", current_user);
                instructorFrame->Show(true);
            }
            
            else
            {
                wxMessageBox("Unknown user role", "Error", wxOK | wxICON_ERROR);
            }

            return;
        }
    }

    wxMessageBox("Invalid username or password", "Error", wxOK | wxICON_ERROR);
}

void LoginFrame::OnCancel(wxCommandEvent& event)
{
    Close(true);  // Close the login window
}