#include "LoginFrame.h"
#include "AdminFrame.h"
#include "InstructorFrame.h"
#include <fstream>
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

    std::ifstream file("users-schema.json");
    if (!file.is_open()) {
        wxMessageBox("Failed to open users-schema.json", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Grab json users
    json users;
    file >> users;
    file.close();
    json current_user;

    for (const auto& user : users)
    {
        if (user["username"] == username.ToStdString() && user["password"] == password.ToStdString())
        {
            current_user = user;
            break;
        }
    }

    wxFrame* frame = nullptr;

    if (current_user["role"] == "admin")
    {
        frame = new AdminFrame(wxT("Admin Dashboard"));
    }
    else if (current_user["role"] == "instructor")
    {
        frame = new InstructorFrame();
    }
    else {
        wxMessageBox("Login successful", "Info", wxOK | wxICON_INFORMATION);
        return;
    }

    if (frame)
    {
        frame->Show();
        Close(true);
    }





    // Look through the users, display correect UI based on role based on the username and password given
    /*for (const auto& user : users) {
        if (user["username"] == username.ToStdString() && user["password"] == password.ToStdString()) {
            // Display Admin frame for admin
            if (user["role"] == "admin") {
                AdminFrame* adminFrame = new AdminFrame(wxT("Admin Dashboard"));
                adminFrame->Show(true);
                Close(true); // Close LoginFrame
                return;
            }
            else if (user["role"] == "instructor") {
                InstructorFrame* instructor_frame = new InstructorFrame(wxT("Instructor Dashboard"));
                instructor_frame->Show(true);
                Close(true);
                return;
            }

            // Display Student/Instructor frame. Possibly single templete for both and pass in the role maybe?
            else {

                wxMessageBox("Login successful", "Info", wxOK | wxICON_INFORMATION);
                return;
            }
        }
    }

    wxMessageBox("Invalid username or password", "Error", wxOK | wxICON_ERROR);*/
}

void LoginFrame::OnCancel(wxCommandEvent& event)
{
    Close(true);  // Close the login window
}
