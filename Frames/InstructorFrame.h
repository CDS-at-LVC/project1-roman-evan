#pragma once
#include <wx/listctrl.h>
#include <wx/wx.h>

class InstructorFrame : public wxFrame
{
public:
    InstructorFrame() : wxFrame(nullptr, wxID_ANY, "Instructor Dashboard", wxDefaultPosition, wxSize(400, 300))
    {
        wxPanel* panel = new wxPanel(this);
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

        // Add instructor-specific buttons
        wxButton* viewStudentsBtn = new wxButton(panel, wxID_ANY, "View Students");
        wxButton* createAssignmentBtn = new wxButton(panel, wxID_ANY, "Create Assignment");

        sizer->Add(viewStudentsBtn, 0, wxALL | wxEXPAND, 5);
        sizer->Add(createAssignmentBtn, 0, wxALL | wxEXPAND, 5);

        // Add a list control for displaying relevant information
        wxListCtrl* listCtrl = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
        sizer->Add(listCtrl, 1, wxALL | wxEXPAND, 5);

        panel->SetSizer(sizer);

        // Bind event handlers
        viewStudentsBtn->Bind(wxEVT_BUTTON, &InstructorFrame::OnViewStudents, this);
        createAssignmentBtn->Bind(wxEVT_BUTTON, &InstructorFrame::OnCreateAssignment, this);
    }

private:
    void OnViewStudents(wxCommandEvent& event) {
        // Implement view students functionality
    }

    void OnCreateAssignment(wxCommandEvent& event) {
        // Implement create assignment functionality
    }
};