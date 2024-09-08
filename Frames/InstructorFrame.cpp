#include "InstructorFrame.h"

InstructorFrame::InstructorFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(400, 300))
{
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // Add instructor-specific buttons
    wxButton* viewStudentsBtn = new wxButton(panel, wxID_ANY, "View Students");
    wxButton* createAssignmentBtn = new wxButton(panel, wxID_ANY, "Create Assignment");

    sizer->Add(viewStudentsBtn, 0, wxALL | wxEXPAND, 5);
    sizer->Add(createAssignmentBtn, 0, wxALL | wxEXPAND, 5);

    // Add a list box for displaying relevant information
    wxListBox* listBox = new wxListBox(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    sizer->Add(listBox, 1, wxALL | wxEXPAND, 5);

    panel->SetSizer(sizer);

    // Bind event handlers
    viewStudentsBtn->Bind(wxEVT_BUTTON, &InstructorFrame::OnViewStudents, this);
    createAssignmentBtn->Bind(wxEVT_BUTTON, &InstructorFrame::OnCreateAssignment, this);
}
