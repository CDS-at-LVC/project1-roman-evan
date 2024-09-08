#pragma once
#include <wx/listctrl.h>
#include <wx/wx.h>

class InstructorFrame : public wxFrame
{
public:
    InstructorFrame(const wxString& title);

private:
    void OnViewStudents(wxCommandEvent& event) {
        // Implement view students functionality
    }

    void OnCreateAssignment(wxCommandEvent& event) {
        // Implement create assignment functionality
    }
};
