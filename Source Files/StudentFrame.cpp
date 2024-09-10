#include "../Header Files/StudentFrame.h"
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/datetime.h>
#include <sstream>
#include <iomanip>

StudentFrame::StudentFrame(const wxString& title)
	: wxFrame(nullptr, wxID_ANY, "Student Dashboard", wxDefaultPosition, wxSize(500, 400))
{
	wxPanel* panel = new wxPanel(this);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	Bind(wxEVT_CLOSE_WINDOW, &StudentFrame::onClose, this);

	load_assignments();
	get_keys(assignmentMap, studentAssignments);
	load_submissions();
	get_keys(submissionMap, studentSubmissions);

	wxNotebook* notebook = new wxNotebook(panel, wxID_ANY);

	// First Tab for assignments
	wxPanel* assignmentPanel = new wxPanel(notebook);
	wxBoxSizer* assignmentSizer = new wxBoxSizer(wxVERTICAL);
	m_assignmentsListBox = new wxListBox(assignmentPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, studentAssignments, wxLB_SINGLE | wxBORDER_SIMPLE);
	wxButton* submitAssignment = new wxButton(assignmentPanel, wxID_ANY, "Submit Assignment");
	wxButton* incompleteAssignmentsBtn = new wxButton(assignmentPanel, wxID_ANY, "Show Incomplete Assignments");
	assignmentSizer->Add(m_assignmentsListBox, 1, wxEXPAND | wxALL, 5);
	assignmentSizer->Add(submitAssignment, 0, wxEXPAND | wxALL, 5);
	assignmentSizer->Add(incompleteAssignmentsBtn, 0, wxEXPAND | wxALL, 5);
	assignmentPanel->SetSizer(assignmentSizer);

	// Second Tab for Submissions
	wxPanel* submissionPanel = new wxPanel(notebook);
	wxBoxSizer* submissionSizer = new wxBoxSizer(wxVERTICAL);
	m_submittedListBox = new wxListBox(submissionPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, studentSubmissions, wxLB_SINGLE | wxBORDER_SIMPLE);
	wxButton* gradeReportBtn = new wxButton(submissionPanel, wxID_ANY, "Get Grade Report");
	submissionSizer->Add(m_submittedListBox, 1, wxEXPAND | wxALL, 5);
	submissionSizer->Add(gradeReportBtn, 0, wxEXPAND | wxALL, 5);
	submissionPanel->SetSizer(submissionSizer);

	// Add tabs to notebook
	notebook->AddPage(assignmentPanel, "Assignments");
	notebook->AddPage(submissionPanel, "Submissions");

	mainSizer->Add(notebook, 1, wxEXPAND | wxALL, 5);
	panel->SetSizer(mainSizer);

	// Bind Event Handlers
	submitAssignment->Bind(wxEVT_BUTTON, &StudentFrame::onSubmitAssignment, this);
	incompleteAssignmentsBtn->Bind(wxEVT_BUTTON, &StudentFrame::onGetIncompleteAssignments, this);
	gradeReportBtn->Bind(wxEVT_BUTTON, &StudentFrame::onGetGradeReport, this);
}

void StudentFrame::load_assignments() {
	std::ifstream file("assignment-schema.json");

	if (!file.is_open()) {
		wxMessageBox("Failed to open assignment-schema.json", "Error ", wxOK | wxICON_ERROR);
		return;
	}

	json assignments;
	file >> assignments;

	for (const auto& assignment : assignments) {
		auto new_assignment = assignment.get<Assignment>();
		assignmentMap[new_assignment.get_assignment_name()] = std::move(new_assignment);
	}

	file.close();
}

void StudentFrame::load_submissions() {
	std::ifstream file("submissions-schema.json");

	if (!file.is_open()) {
		wxMessageBox("Failed to open submissions-schema.json", "Error ", wxOK | wxICON_ERROR);
		return;
	}

	json submissions;
	file >> submissions;

	for (const auto& submission : submissions) {
		auto new_submission = submission.get<Submission>();
		submissionMap[new_submission.get_assignment_name()] = std::move(new_submission);
	}

	file.close();
}

void StudentFrame::onSubmitAssignment(wxCommandEvent& event) {
	// Get the selected assignment
	int selectedIndex = m_assignmentsListBox->GetSelection();
	if (selectedIndex == wxNOT_FOUND) {
		wxMessageBox("Please select an assignment to submit.", "Error", wxOK | wxICON_ERROR);
		return;
	}

	wxString selectedAssignmentName = m_assignmentsListBox->GetString(selectedIndex);
	Assignment& selectedAssignment = assignmentMap[selectedAssignmentName.ToStdString()];

	// Check if the assignment is past due
	wxDateTime now = wxDateTime::Now();
	wxDateTime dueDate;
	dueDate.ParseISOCombined(selectedAssignment.get_due_date());

	if (now > dueDate) {
		wxMessageBox("This assignment is past due and cannot be submitted.", "Submission Error", wxOK | wxICON_ERROR);
		return;
	}

	// Open file dialog for selecting submission file
	wxFileDialog submissionDialog(this, "Choose Submission File", "", "",
		"Text files (*.txt)|*.txt",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (submissionDialog.ShowModal() == wxID_CANCEL) {
		return;  // User cancelled the file dialog
	}

	// Get the selected file path
	wxString filePath = submissionDialog.GetPath();

	// TODO: Implement file copying logic here
	// For now, we'll just create a submission object

	// Create a new submission object
	Submission newSubmission(
		selectedAssignment.get_assignment_id(),
		selectedAssignment.get_assignment_name(),
		"current_username",  // TODO: Replace with actual username
		true,  // accepted
		false,  // passed (to be determined after grading)
		0,  // tests_passed (to be determined after grading)
		selectedAssignment.get_inputs().size(),  // total_tests
		now.FormatISOCombined().ToStdString()  // submission_time
	);

	// Update or add the submission to the submissionMap
	submissionMap[selectedAssignment.get_assignment_name()] = std::move(newSubmission);

	// Update the submissions list box
	UpdateSubmissionsList();

	// Save the updated submissions to file
	SaveSubmissions();

	wxMessageBox("Assignment submitted successfully!", "Submission", wxOK | wxICON_INFORMATION);
}

void StudentFrame::UpdateSubmissionsList() {
	studentSubmissions.Clear();
	for (const auto& pair : submissionMap) {
		studentSubmissions.Add(wxString(pair.first));
	}
	m_submittedListBox->Set(studentSubmissions);
}

void StudentFrame::SaveSubmissions() {
	json j_submissions = json::array();

	for (const auto& pair : submissionMap) {
		j_submissions.push_back(pair.second);
	}

	std::ofstream file("submissions-schema.json");
	if (file.is_open()) {
		file << j_submissions.dump(4);  // Dump the JSON with 4-space indentation
		file.close();
	}
	else {
		wxMessageBox("Failed to save submission data to file.", "Error", wxOK | wxICON_ERROR);
	}
}

void StudentFrame::onClose(wxCloseEvent& event) {
	if (assignmentMap.empty()) {
		event.Skip();
		return;
	}

	json j_users = json::array();

	for (const auto& pair : assignmentMap) {
		j_users.push_back(pair.second);

		std::ofstream file("assignment-schema.json");
		if (file.is_open()) {
			file << j_users.dump(4);  // Dump the JSON with 4-space indentation
			file.close();
		}
		else {
			wxMessageBox("Failed to save user data to file.", "Error", wxOK | wxICON_ERROR);
		}

		// Continue with the default close behavior
		event.Skip();  // Ensures the window will actually close
	}
}

void StudentFrame::onGetGradeReport(wxCommandEvent& event) {
	std::stringstream report;
	report << "Grade Report\n\n";
	report << std::left << std::setw(20) << "Assignment" << std::setw(10) << "Grade" << std::setw(20) << "Submission Time" << "\n";
	report << std::string(50, '-') << "\n";

	for (const auto& pair : submissionMap) {
		const Submission& submission = pair.second;
		double grade = (submission.get_tests_passed() * 100.0) / submission.get_total_tests();
		report << std::left << std::setw(20) << submission.get_assignment_name()
			<< std::setw(10) << std::fixed << std::setprecision(2) << grade << "%"
			<< std::setw(20) << submission.get_submission_time() << "\n";
	}

	wxMessageBox(report.str(), "Grade Report", wxOK | wxICON_INFORMATION);
}

void StudentFrame::onGetIncompleteAssignments(wxCommandEvent& event) {
	std::vector<wxString> incompleteAssignments;

	for (const auto& pair : assignmentMap) {
		if (submissionMap.find(pair.first) == submissionMap.end()) {
			incompleteAssignments.push_back(pair.first);
		}
	}

	if (incompleteAssignments.empty()) {
		wxMessageBox("No incomplete assignments found.", "Incomplete Assignments", wxOK | wxICON_INFORMATION);
	}
	else {
		std::stringstream report;
		report << "Incomplete Assignments\n\n";
		for (const auto& assignment : incompleteAssignments) {
			report << assignment << "\n";
		}
		wxMessageBox(report.str(), "Incomplete Assignments", wxOK | wxICON_INFORMATION);
	}

