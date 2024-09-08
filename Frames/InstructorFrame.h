#pragma once
#include <wx/listctrl.h>
#include <wx/wx.h>
#include <fstream>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <vector>
#include <wx/datectrl.h>
#include <wx/aui/auibook.h>
#include "../Models/User.h"
#include "../Util/Util.h"

using json = nlohmann::json;

class InstructorFrame : public wxFrame
{
public:
    InstructorFrame(const wxString& title);

private:
    wxListBox* m_studentsListBox;
    wxListBox* m_assignmentsListBox;
    wxArrayString studentUsernames;
    std::unordered_map<wxString, User> studentsMap;
    std::unordered_map<wxString, User> assignmentsMap;

    void load_students();
    void load_assignments();


    void OnViewStudents(wxCommandEvent& event) {
        // Implement view students functionality
    }

    void OnCreateAssignment(wxCommandEvent& event) {
        // Implement create assignment functionality
    }
};
