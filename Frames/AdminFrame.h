
#ifndef ADMINFRAME_H
#define ADMINFRAME_H

#include <wx/wx.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>

using json = nlohmann::json;

class AdminFrame : public wxFrame {
public:
    AdminFrame(const wxString& title);

    std::vector<std::string> get_users();

    void createUser(std::string username, std::string role, std::string password);
    void deleteUser(std::string username);
    void onCreateUser(wxCommandEvent& event);
    void onDeleteUser(wxCommandEvent& event);

private:
    std::vector<std::string> usersVector;
    wxListBox* userList;
};

#endif // ADMINFRAME_H