#include "LoginFrame.h"
#include "AdminFrame.h"
#include "InstructorFrame.h"
#include "../Models/User.h"
#include "StudentFrame.h"

#define _CRT_SECURE_NO_WARNINGS

LoginFrame::LoginFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(300, 200))
{
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    wxStaticText* userLabel = new wxStaticText(panel, wxID_ANY, wxT("Username:"), wxPoint(20, 20));
    userTextCtrl = new wxTextCtrl(panel, wxID_ANY, wxT(""), wxPoint(100, 20), wxSize(150, 25));

    wxStaticText* passLabel = new wxStaticText(panel, wxID_ANY, wxT("Password:"), wxPoint(20, 60));
    passTextCtrl = new wxTextCtrl(panel, wxID_ANY, wxT(""), wxPoint(100, 60), wxSize(150, 25), wxTE_PASSWORD);

    wxButton* loginBtn = new wxButton(panel, wxID_ANY, wxT("Login"), wxPoint(60, 110));
    wxButton* cancelBtn = new wxButton(panel, wxID_ANY, wxT("Cancel"), wxPoint(150, 110));

    loginBtn->Bind(wxEVT_BUTTON, &LoginFrame::OnLogin, this);
    cancelBtn->Bind(wxEVT_BUTTON, &LoginFrame::OnCancel, this);
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

    std::ifstream file("jsonSchemas/users-schema.json");
    if (!file.is_open()) {
        wxMessageBox("Failed to open users-schema.json", "Error", wxOK | wxICON_ERROR);
        return;
    }

    json users;
    file >> users;
    file.close();
    User current_user;

    for (const auto& user : users)
    {
        if (user["username"] == username.ToStdString() && user["password"] == password.ToStdString())
        {
            current_user = user;
            break;
        }
    }

    if (current_user.get_username().empty()) {
        wxMessageBox("Invalid credentials entered", "Error", wxOK | wxICON_ERROR);
        return;
    }

    wxFrame* frame = nullptr;
    std::string role = current_user.get_role();

    if (role == "admin")
    {
        frame = new AdminFrame(wxT("Admin Dashboard"), std::move(current_user));
    }
    else if (role == "instructor")
    {
        frame = new InstructorFrame(wxT("Instructor Dashboard"));
    }
    else if (role == "student") 
    {
        frame = new StudentFrame(wxT("Student Dasboard"), std::move(current_user));
    }

    if (frame)
    {
        frame->Show();
        Close(true);
    }
}

void LoginFrame::OnCancel(wxCommandEvent& event)
{
    Close(true);  // Close the login window
}