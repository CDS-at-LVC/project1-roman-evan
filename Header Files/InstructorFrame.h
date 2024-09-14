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
#include <wx/grid.h>
#include "User.h"
#include "Assignment.h"
#include "Util.h"
#include "Submission.h"
#include "AddAssignmentFrame.h"

using json = nlohmann::json;

class InstructorFrame : public wxFrame
{
public:
    InstructorFrame(const wxString& title);

private:
    wxGrid* m_assignmentsGrid;

    wxListBox* m_studentsListBox;
    wxListBox* m_assignmentsListBox;
    wxArrayString studentUsernames;
    wxArrayString assignmentsArray;
    std::unordered_map<wxString, User> studentsMap;
    std::unordered_map<wxString, Assignment> assignmentsMap;

    void load_students();
    void load_assignments();
    void load_submissions(std::vector<Submission>& submissions);

    void OnAddAssignment(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnGridSelect(wxGridEvent& event);
    void UpdateAssignmentssList();
    void OnDeleteAssignment(wxCommandEvent& event);

    void OnGetAssignmentReport(wxCommandEvent& event);
};