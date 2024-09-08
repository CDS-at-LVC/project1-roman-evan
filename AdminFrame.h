
#ifndef ADMINFRAME_H
#define ADMINFRAME_H

#include <wx/wx.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include "User.h"

using json = nlohmann::json;

class AdminFrame : public wxFrame {
public:
    AdminFrame(const wxString& title, const User& current_user);

    std::vector<User> get_users();

    void createUser(const std::string& username, const std::string& role, const std::string& password);
    void deleteUser(const std::string& username);
    void onCreateUser(wxCommandEvent& event);
    void onDeleteUser(wxCommandEvent& event);

private:
    User current_user;
    std::vector<User> usersVector;
    wxListBox* userList;
};

#endif // ADMINFRAME_H