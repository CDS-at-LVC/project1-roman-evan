#include "InstructorFrame.h"

InstructorFrame::InstructorFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, "Instructor Dashboard", wxDefaultPosition, wxSize(500, 400))
{
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    load_students();
    get_keys(studentsMap, studentUsernames);
    load_assignments();

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

    wxButton* deleteAssignmentBtn = new wxButton(assignmentsPanel, wxID_ANY, "Delete Assignment");
    assignmentsSizer->Add(addAssignmentBtn, 0, wxEXPAND | wxALL, 5);


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
    deleteAssignmentBtn->Bind(wxEVT_BUTTON, &InstructorFrame::onDeleteAssignment, this);

    //refreshBtn->Bind(wxEVT_BUTTON, &InstructorFrame::OnRefresh, this);
    addAssignmentBtn->Bind(wxEVT_BUTTON, &InstructorFrame::OnCreateAssignment, this);
}

void InstructorFrame::load_students()
{
    std::ifstream file("users-schema.json");

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

void InstructorFrame::load_assignments() {
    std::ifstream file("assignment-schema.json");

    if (!file.is_open()) {
        wxMessageBox("Failed to open assignments-schema.json", "Error", wxOK | wxICON_ERROR);
        return;
    }

    json assignments_json;
    file >> assignments_json;
    file.close();

    m_assignmentsListBox->Clear();
    assignmentsMap.clear();

    for (const auto& assignment_json : assignments_json) {
        // Create an Assignment object from JSON using from_json
        Assignment assignment;
        from_json(assignment_json, assignment);


        // Add assignment to map using assignment ID as key
        assignmentsMap[assignment.get_assignment_name()] = assignment;

        // Convert std::string to wxString
        wxString assignment_name = wxString(assignment.get_assignment_name());
        wxString due_date = wxString(assignment.get_due_date());

        // Format and add the assignment details to the ListBox
        wxString assignment_details = wxString::Format("%s (Due: %s)",
            assignment_name,
            due_date);
        m_assignmentsListBox->Append(assignment_details);
    }
}

void InstructorFrame::OnCreateAssignment(wxCommandEvent& event) {
    AddAssignmentDialog dialog(this);
    if (dialog.ShowModal() == wxID_OK) {
        // Get the assignment details from the dialog
        std::string id = dialog.GetAssignmentID();
        std::string name = dialog.GetAssignmentName();
        std::string due_date = dialog.GetDueDate();
        std::vector<std::string> input_files = dialog.GetInputFiles();
        std::vector<std::string> output_files = dialog.GetOutputFiles();

        // Validate that the number of input files matches the number of output files
        if (input_files.size() != output_files.size()) {
            wxMessageBox("The number of input files must match the number of output files.", "Error", wxOK | wxICON_ERROR);
            return;
        }

        // Create the Assignment object
        Assignment newAssignment(id, name, due_date, input_files, output_files);

        // Load existing assignments from file
        std::ifstream file_in("assignment-schema.json");
        json assignments_json;

        if (file_in.is_open()) {
            file_in >> assignments_json;
            file_in.close();
        }

        // Check if the assignment already exists and update or add it
        bool found = false;
        for (auto& assignment_json : assignments_json) {
            if (assignment_json.at("id").get<std::string>() == id) {
                to_json(assignment_json, newAssignment);
                found = true;
                break;
            }
        }

        if (!found) {
            json new_assignment_json;
            to_json(new_assignment_json, newAssignment);
            assignments_json.push_back(new_assignment_json);
        }

        // Write the updated assignments back to the file
        std::ofstream file_out("assignment-schema.json");
        if (!file_out.is_open()) {
            wxMessageBox("Failed to open assignments file for writing.", "Error", wxOK | wxICON_ERROR);
            return;
        }

        file_out << std::setw(4) << assignments_json << std::endl;
        file_out.close();

        wxMessageBox("Assignment added successfully.", "Success", wxOK | wxICON_INFORMATION);
    }
}

void InstructorFrame::OnCreateAssignment(wxCommandEvent& event) {
    AddAssignmentDialog dialog(this);
    if (dialog.ShowModal() == wxID_OK) {
        // Get the assignment details from the dialog
        std::string id = dialog.GetAssignmentID();
        std::string name = dialog.GetAssignmentName();
        std::string due_date = dialog.GetDueDate();
        std::vector<std::string> input_files = dialog.GetInputFiles();
        std::vector<std::string> output_files = dialog.GetOutputFiles();

        // Validate that the number of input files matches the number of output files
        if (input_files.size() != output_files.size()) {
            wxMessageBox("The number of input files must match the number of output files.", "Error", wxOK | wxICON_ERROR);
            return;
        }

        // Create the Assignment object
        Assignment newAssignment(id, name, due_date, input_files, output_files);

        // Load existing assignments from file
        std::ifstream file_in("assignment-schema.json");
        json assignments_json;

        if (file_in.is_open()) {
            file_in >> assignments_json;
            file_in.close();
        }

        // Check if the assignment already exists and update or add it
        bool found = false;
        for (auto& assignment_json : assignments_json) {
            if (assignment_json.at("id").get<std::string>() == id) {
                to_json(assignment_json, newAssignment);
                found = true;
                break;
            }
        }

        if (!found) {
            json new_assignment_json;
            to_json(new_assignment_json, newAssignment);
            assignments_json.push_back(new_assignment_json);
        }

        // Write the updated assignments back to the file
        std::ofstream file_out("assignment-schema.json");
        if (!file_out.is_open()) {
            wxMessageBox("Failed to open assignments file for writing.", "Error", wxOK | wxICON_ERROR);
            return;
        }

        file_out << std::setw(4) << assignments_json << std::endl;
        file_out.close();

        wxMessageBox("Assignment added successfully.", "Success", wxOK | wxICON_INFORMATION);
    }
}

