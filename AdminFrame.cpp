#include "AdminFrame.h"
#include <wx/wx.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <algorithm>

using json = nlohmann::json;

AdminFrame::AdminFrame(const wxString& title, const User& current_user)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(400, 300)), current_user(current_user)
{
    // Create the main panel
    wxPanel* admin_panel = new wxPanel(this, wxID_ANY);
    admin_panel->SetBackgroundColour(wxColour(240, 240, 240));  // Light gray background

    // Create buttons for "Create User" and "Delete User"
    wxButton* create_user_button = new wxButton(admin_panel, wxID_ANY, "Create User", wxDefaultPosition, wxSize(100, 30));
    wxButton* delete_user_button = new wxButton(admin_panel, wxID_ANY, "Delete User", wxDefaultPosition, wxSize(100, 30));

    // Bind events to the buttons
    create_user_button->Bind(wxEVT_BUTTON, &AdminFrame::onCreateUser, this);
    delete_user_button->Bind(wxEVT_BUTTON, &AdminFrame::onDeleteUser, this);

    // Get the user list from the JSON file
    usersVector = get_users();

    wxArrayString wxUserArray;
    for (const auto& user : usersVector) {
        wxUserArray.Add(user.get_username());
    }

    // Create a styled wxListBox to display users
    userList = new wxListBox(admin_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxUserArray, wxLB_SINGLE | wxBORDER_SIMPLE);
    userList->SetBackgroundColour(wxColour(255, 255, 255));  // White background for the list
    userList->SetForegroundColour(wxColour(0, 0, 0));        // Black text color
    userList->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    // Use a sizer to manage layout
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(create_user_button, 0, wxRIGHT, 10);
    buttonSizer->Add(delete_user_button, 0);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->AddSpacer(20); // Add space at the top
    mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 20); // Align buttons to the right
    mainSizer->Add(userList, 1, wxALL | wxEXPAND, 20);  // Add the list with padding

    // Set the sizer for the panel
    admin_panel->SetSizer(mainSizer);

    // Center the frame on the screen
    Centre();
}

std::vector<User> AdminFrame::get_users() {
    std::vector<User> users;
    std::ifstream file("users-schema.json");

    if (!file.is_open()) {
        wxMessageBox("Failed to open users-schema.json", "Error ", wxOK | wxICON_ERROR);
        return users;
    }

    json json_users;
    file >> json_users;

    for (const auto& user : json_users) {
        if (user["role"] != "admin") {
            users.emplace_back(user["username"], user["password"], user["role"]);
        }
    }

    file.close();
    return users;
}

void AdminFrame::createUser(const std::string& username, const std::string& role, const std::string& password) {
    std::ifstream file("users-schema.json");
    if (!file.is_open()) {
        wxMessageBox("Failed to open users-schema.json", "Error", wxOK | wxICON_ERROR);
        return;
    }

    json users;
    file >> users;
    file.close();

    // Check to see if username already exists
    if (std::any_of(usersVector.begin(), usersVector.end(),
        [&username](const User& user) { return user.get_username() == username; })) {
        wxMessageBox("Username already exists!", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Create and add new user to json file
    json newUser = {
        {"username", username},
        {"role", role},
        {"password", password},
        {"completed", json::array()}
    };

    users.push_back(newUser);

    std::ofstream outFile("users-schema.json");
    outFile << std::setw(4) << users << std::endl;
    outFile.close();

    // Add user to admin vector of usernames to be displayed, assuming we dont want admin to control other admin
    if (role != "admin") {
        usersVector.emplace_back(username, password, role);
        userList->AppendString(username);  // Update the wxListBox with the new user
    }

    wxMessageBox("User created successfully!", "Success", wxOK | wxICON_INFORMATION);
}

void AdminFrame::deleteUser(const std::string& username) {
    std::ifstream file("users-schema.json");
    if (!file.is_open()) {
        wxMessageBox("Failed to open users-schema.json", "Error", wxOK | wxICON_ERROR);
        return;
    }

    json users;
    file >> users;
    file.close();

    users.erase(std::remove_if(users.begin(), users.end(),
        [&username](const json& user) { return user["username"] == username; }),
        users.end());

    std::ofstream outFile("users-schema.json");
    outFile << std::setw(4) << users << std::endl;
    outFile.close();

    // delete from admins user vector
    auto it = std::find_if(usersVector.begin(), usersVector.end(),
        [&username](const User& user) { return user.get_username() == username; });
    if (it != usersVector.end()) {
        usersVector.erase(it);
        userList->Delete(userList->FindString(username));  // Remove the user from the wxListBox
    }

    wxMessageBox("User deleted successfully!", "Success", wxOK | wxICON_INFORMATION);
}

void AdminFrame::onCreateUser(wxCommandEvent& event) {
    wxTextEntryDialog usernameDlg(this, "Enter username:", "Create User");
    if (usernameDlg.ShowModal() == wxID_OK) {
        std::string username = usernameDlg.GetValue().ToStdString();

        wxTextEntryDialog roleDlg(this, "Enter role (student, instructor, admin):", "Create User");
        if (roleDlg.ShowModal() == wxID_OK) {
            std::string role = roleDlg.GetValue().ToStdString();

            wxTextEntryDialog passwordDlg(this, "Enter password:", "Create User", wxEmptyString, wxTE_PASSWORD);
            if (passwordDlg.ShowModal() == wxID_OK) {
                std::string password = passwordDlg.GetValue().ToStdString();
                createUser(username, role, password);
            }
        }
    }
}

void AdminFrame::onDeleteUser(wxCommandEvent& event) {
    wxString selectedUser = userList->GetStringSelection();
    if (selectedUser.IsEmpty()) {
        wxMessageBox("Please select a user to delete.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    wxMessageDialog confirmDlg(this, "Are you sure you want to delete the user '" + selectedUser + "'?", "Confirm Deletion", wxYES_NO | wxNO_DEFAULT | wxICON_WARNING);
    if (confirmDlg.ShowModal() == wxID_YES) {
        deleteUser(selectedUser.ToStdString());
    }
}