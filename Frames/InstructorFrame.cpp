#include "InstructorFrame.h"

InstructorFrame::InstructorFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, "Instructor Dashboard", wxDefaultPosition, wxSize(500, 400))
{
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    load_students();
    get_keys(studentsMap, studentUsernames);

    // Create notebook (tabbed interface)
    wxNotebook* notebook = new wxNotebook(panel, wxID_ANY);

    // Create first tab for students
    wxPanel* studentsPanel = new wxPanel(notebook);
    wxBoxSizer* studentsSizer = new wxBoxSizer(wxVERTICAL);
    m_studentsListBox = new wxListBox(studentsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, studentUsernames, wxLB_SINGLE | wxBORDER_SIMPLE);
    wxButton* refreshBtn = new wxButton(studentsPanel, wxID_ANY, "Get Student Report");
    studentsSizer->Add(m_studentsListBox, 1, wxEXPAND | wxALL, 5);
    studentsSizer->Add(refreshBtn, 0, wxEXPAND | wxALL, 5);
    studentsPanel->SetSizer(studentsSizer);

    // Create second tab for assignments
    wxPanel* assignmentsPanel = new wxPanel(notebook);
    wxBoxSizer* assignmentsSizer = new wxBoxSizer(wxVERTICAL);
    m_assignmentsListBox = new wxListBox(assignmentsPanel, wxID_ANY);
    wxButton* addAssignmentBtn = new wxButton(assignmentsPanel, wxID_ANY, "Add Assignment");
    assignmentsSizer->Add(m_assignmentsListBox, 1, wxEXPAND | wxALL, 5);
    assignmentsSizer->Add(addAssignmentBtn, 0, wxEXPAND | wxALL, 5);
    assignmentsPanel->SetSizer(assignmentsSizer);

    // Add tabs to notebook
    notebook->AddPage(studentsPanel, "Students");
    notebook->AddPage(assignmentsPanel, "Assignments");

    // Add notebook to main sizer
    mainSizer->Add(notebook, 1, wxEXPAND | wxALL, 5);

    // Add buttons
  

    //mainSizer->Add(refreshBtn, 0, wxALL | wxEXPAND, 5);
    //mainSizer->Add(addAssignmentBtn, 0, wxALL | wxEXPAND, 5);

    panel->SetSizer(mainSizer);

    // Bind event handlers
    //refreshBtn->Bind(wxEVT_BUTTON, &InstructorFrame::OnRefresh, this);
    //addAssignmentBtn->Bind(wxEVT_BUTTON, &InstructorFrame::OnAddAssignment, this);
}

void InstructorFrame::load_students()
{
    std::ifstream file("jsonSchemas/users-schema.json");

    if (!file.is_open()) {
        wxMessageBox("Failed to open users-schema.json", "Error ", wxOK | wxICON_ERROR);
        return;
    }

    json users;
    file >> users;

    for (const auto& user : users) {
        auto new_user = user.get<User>();
        if (new_user.get_role() == "student") {
            studentsMap[new_user.get_username()] = std::move(new_user);
        }
    }

    file.close();
}
