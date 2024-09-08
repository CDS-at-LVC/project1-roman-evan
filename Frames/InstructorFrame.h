#pragma once
#include <wx/listctrl.h>
#include <wx/wx.h>
#include <wx/datectrl.h>
#include <wx/aui/auibook.h>

class InstructorFrame : public wxFrame
{
public:
    InstructorFrame(const wxString& title);

private:
    wxListBox* m_studentsListBox;
    wxListBox* m_assignmentsListBox;


    void OnViewStudents(wxCommandEvent& event) {
        // Implement view students functionality
    }

    void OnCreateAssignment(wxCommandEvent& event) {
        // Implement create assignment functionality
    }
};
