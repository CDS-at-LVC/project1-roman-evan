#include <wx/wx.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <streambuf>

// Define the login window class
class LoginFrame : public wxFrame
{
public:
    LoginFrame(const wxString& title)
        : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(300, 200))
    {
        // Create a panel inside the frame
        wxPanel* panel = new wxPanel(this, wxID_ANY);

        // Create controls
        wxStaticText* userLabel = new wxStaticText(panel, wxID_ANY, wxT("Username:"), wxPoint(20, 20));
        userTextCtrl = new wxTextCtrl(panel, wxID_ANY, wxT(""), wxPoint(100, 20), wxSize(150, 25));

        wxStaticText* passLabel = new wxStaticText(panel, wxID_ANY, wxT("Password:"), wxPoint(20, 60));
        passTextCtrl = new wxTextCtrl(panel, wxID_ANY, wxT(""), wxPoint(100, 60), wxSize(150, 25), wxTE_PASSWORD);

        wxButton* loginBtn = new wxButton(panel, wxID_ANY, wxT("Login"), wxPoint(60, 110));
        wxButton* cancelBtn = new wxButton(panel, wxID_ANY, wxT("Cancel"), wxPoint(150, 110));

        // Bind events to buttons
        loginBtn->Bind(wxEVT_BUTTON, &LoginFrame::OnLogin, this);
        cancelBtn->Bind(wxEVT_BUTTON, &LoginFrame::OnCancel, this);
    }

private:
    wxTextCtrl* userTextCtrl; // To hold username input
    wxTextCtrl* passTextCtrl; // To hold password input

    // Event handler for login button
    void OnLogin(wxCommandEvent& event)
    {
        wxString username = userTextCtrl->GetValue();
        wxString password = passTextCtrl->GetValue();

        auto pass = password.ToStdString();

        std::ifstream input{ "users-schema.json" };

        std::string fileText((std::istreambuf_iterator<char>(input)),
            std::istreambuf_iterator<char>());

        using json = nlohmann::json;

        json j = json::parse(fileText);

        for (auto start = j.begin(); start != j.end(); start++) {
            if (username.IsSameAs(start->at("username").get<std::string>()) && password.IsSameAs(start->at("password").get<std::string>())) {
                wxMessageBox("Login Successful", "Info", wxOK | wxICON_INFORMATION);
                this->Show(false);
                return;
            }
        }

        if (username.IsEmpty() || password.IsEmpty())
        {
            wxMessageBox("Please enter both username and password", "Error", wxOK | wxICON_ERROR);
            return;
        }

        // Implement your login logic here
        
    }

    // Event handler for cancel button
    void OnCancel(wxCommandEvent& event)
    {
        Close(true);  // Close the login window
    }
};

// Define the main application class
class MyApp : public wxApp
{
public:
    virtual bool OnInit()
    {
        LoginFrame* loginFrame = new LoginFrame(wxT("Login"));
        loginFrame->Show(true);
        return true;
    }
};

// Implement the application
wxIMPLEMENT_APP(MyApp);