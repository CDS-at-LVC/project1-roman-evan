#pragma once
#include <wx/wx.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <unordered_map>
#include <vector>
#include "../Models/User.h"

using json = nlohmann::json;

class AdminFrame : public wxFrame {
public:
    AdminFrame(const wxString& title, User adminUser);

    void load_users();

    void createUser(const std::string& username, const std::string& role, const std::string& password);
    //void deleteUser(std::string username);
    void onCreateUser(wxCommandEvent& event);
    //void onDeleteUser(wxCommandEvent& event);

private:
    void OnClose(wxCloseEvent& event);

    std::unordered_map<wxString, User> usersMap;
    User currentUser;
    wxListBox* userList;
    wxArrayString wxUserArray;
};