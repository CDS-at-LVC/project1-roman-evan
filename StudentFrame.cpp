#include <wx/wx.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <ctime>
#include "StudentFrame.h"


wxBEGIN_EVENT_TABLE(StudentFrame, wxFrame)
    EVT_BUTTON(wxID_ANY, StudentFrame::onSubmitAssignment)
wxEND_EVENT_TABLE()

StudentFrame::StudentFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(400, 300)) 
{
    wxPanel* student_panel = new wxPanel(this, wxID_ANY);
    wxButton* submit_assignment_button = new wxButton(student_panel, wxID_ANY, "Submit Assignment", wxDefaultPosition, wxSize(100, 30));

    assignments = load_assignments();


    wxArrayString wxAssignmentArray;
    for (const auto& assignment : assignments) {
        wxAssignmentArray.Add(assignment.assignment_name);
    }

    assignment_list = new wxListBox(student_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAssignmentArray, wxLB_SINGLE | wxBORDER_SIMPLE);
    assignment_list->SetBackgroundColour(wxColour(255, 255, 255));  // White background for the list
    assignment_list->SetForegroundColour(wxColour(0, 0, 0));        // Black text color
    assignment_list->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    wxButton* view_report_button = new wxButton(student_panel, wxID_ANY, "View Grade Report", wxDefaultPosition, wxSize(150, 30));

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    vbox->Add(assignment_list, 1, wxEXPAND | wxALL, 10);
    vbox->Add(submit_assignment_button, 0, wxALIGN_CENTER | wxALL, 10);
    vbox->Add(view_report_button, 0, wxALIGN_CENTER | wxALL, 10);
    student_panel->SetSizer(vbox);
}

std::vector<StudentFrame::Assignment> StudentFrame::load_assignments() {
    std::vector<Assignment> assignments;
    // Simulated loading of assignments
    assignments.push_back({ "Assignment 1", std::time(nullptr) + 60 * 60 * 24 * 2 }); // Due in 2 days
    assignments.push_back({ "Assignment 2", std::time(nullptr) + 60 * 60 * 24 * 5 }); // Due in 5 days
    return assignments;
}

void StudentFrame::onSubmitAssignment(wxCommandEvent& event) {
    int selected = assignment_list->GetSelection();
    if (selected == wxNOT_FOUND) {
        wxMessageBox("Please select an assignment.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    Assignment selected_assignment = assignments[selected];

    if (!check_due_date(selected_assignment)) {
        wxMessageBox("Submission rejected: The due date has passed.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    wxFileDialog openFileDialog(this, _("Select source code file"), "", "", "Source files (*.cpp;*.h)|*.cpp;*.h", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;  // the user changed their mind

    std::string file_path = openFileDialog.GetPath().ToStdString();

    double grade = compile_and_grade(file_path);

    Submission new_submission;
    new_submission.assigned = selected_assignment;
    new_submission.compiled = (grade != -1);
    new_submission.result_of_compilation = new_submission.compiled ? "Compiled successfully" : "Compilation failed";
    new_submission.grade = new_submission.compiled ? grade : 0;
    new_submission.passed_due_date = !check_due_date(selected_assignment);

    submissions.push_back(new_submission);

    wxMessageBox(new_submission.result_of_compilation + "\nGrade: " + std::to_string(new_submission.grade), "Submission Result", wxOK | wxICON_INFORMATION);
}

bool StudentFrame::check_due_date(const Assignment& assignment) {
    time_t now = std::time(nullptr);
    return difftime(assignment.assignment_due_date, now) > 0;
}

double StudentFrame::compile_and_grade(const std::string& file_path) {
    // Simulate the compilation and grading process
    // In a real system, this would invoke the compiler and run tests
    std::ifstream file(file_path);

    if (!file.good()) {
        return -1;  // Compilation failed
    }

    // Simulate grading (just random grade for now)
    return 60 + rand() % 40;  // Grade between 60 and 100
}

void StudentFrame::get_grade_report() {
    wxString report = "Grade Report:\n";
    for (const auto& submission : submissions) {
        report += submission.assigned.assignment_name + ": " + (submission.compiled ? std::to_string(submission.grade) : "Failed to compile") + "\n";
    }

    wxMessageBox(report, "Grade Report", wxOK | wxICON_INFORMATION);
}

void StudentFrame::list_pending_assignments() {
    wxString pending = "Pending Assignments:\n";
    for (const auto& assignment : assignments) {
        bool submitted = false;
        for (const auto& submission : submissions) {
            if (submission.assigned.assignment_name == assignment.assignment_name) {
                submitted = true;
                break;
            }
        }
        if (!submitted) {
            pending += assignment.assignment_name + "\n";
        }
    }

    wxMessageBox(pending, "Pending Assignments", wxOK | wxICON_INFORMATION);
}
  




