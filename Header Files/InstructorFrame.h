#pragma once
#include <wx/listctrl.h>
#include <wx/wx.h>
#include <fstream>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <vector>
#include <wx/datectrl.h>
#include <wx/aui/auibook.h>
#include <filesystem>
#include <iostream>
#include <set>
#include "User.h"
#include "Assignment.h"
#include "Util.h"
#include "AddAssignmentFrame.h"
#include "Submission.h"

using json = nlohmann::json;

class InstructorFrame : public wxFrame
{
public:
    InstructorFrame(const wxString& title);

private:
    wxListBox* m_studentsListBox;
    wxListBox* m_assignmentsListBox;
    wxArrayString studentUsernames;
    wxArrayString assignmentsArray;
    std::unordered_map<wxString, User> studentsMap;
    std::unordered_map<wxString, Assignment> assignmentsMap;
    std::unordered_map<wxString, Submission> submissionsMap;  

    void load_students();
    void load_assignments();
    void load_submissions();
    void OnAddAssignment(wxCommandEvent& event);
    void OnDeleteAssignment(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnGetAssignmentReport(wxCommandEvent& event);
};