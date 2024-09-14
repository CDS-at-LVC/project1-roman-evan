#include "../Header Files/StudentFrame.h"

StudentFrame::StudentFrame(const wxString& title, User studentUser)
	: wxFrame(nullptr, wxID_ANY, "Student Dashboard", wxDefaultPosition, wxSize(500, 400))
{
	currentUser = std::move(studentUser);

	wxPanel* panel = new wxPanel(this);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	//Bind(wxEVT_CLOSE_WINDOW, &StudentFrame::onClose, this);

	wxNotebook* notebook = new wxNotebook(panel, wxID_ANY);

	// First Tab for assignments
	wxPanel* assignmentPanel = new wxPanel(notebook);
	wxBoxSizer* assignmentSizer = new wxBoxSizer(wxVERTICAL);
	//m_assignmentsListBox = new wxListBox(assignmentPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, studentAssignments, wxLB_SINGLE | wxBORDER_SIMPLE);
	m_assignmentsGrid = new wxGrid(assignmentPanel, wxID_ANY);
	m_assignmentsGrid->CreateGrid(0, 2);
	m_assignmentsGrid->SetColLabelValue(0, "Assignment ID");
	m_assignmentsGrid->SetColLabelValue(1, "Due Date");
	m_assignmentsGrid->AutoSizeColumns();
	m_assignmentsGrid->AutoSizeRows();
	m_assignmentsGrid->EnableEditing(false);
	m_assignmentsGrid->SetSelectionMode(wxGrid::wxGridSelectRows);
	m_assignmentsGrid->HideRowLabels();
	m_assignmentsGrid->SetGridLineColour(m_assignmentsGrid->GetBackgroundColour());
	wxButton* submitAssignment = new wxButton(assignmentPanel, wxID_ANY, "Submit Assignment");
	wxButton* incompleteAssignmentsBtn = new wxButton(assignmentPanel, wxID_ANY, "Show Incomplete Assignments");
	//assignmentSizer->Add(m_assignmentsListBox, 1, wxEXPAND | wxALL, 5);
	assignmentSizer->Add(m_assignmentsGrid, 1, wxEXPAND | wxALL, 5);
	assignmentSizer->Add(submitAssignment, 0, wxEXPAND | wxALL, 5);
	assignmentSizer->Add(incompleteAssignmentsBtn, 0, wxEXPAND | wxALL, 5);
	assignmentPanel->SetSizer(assignmentSizer);

	// Second Tab for Submissions
	wxPanel* submissionPanel = new wxPanel(notebook);
	wxBoxSizer* submissionSizer = new wxBoxSizer(wxVERTICAL);
	m_submissionsGrid = new wxGrid(submissionPanel, wxID_ANY);
	m_submissionsGrid->CreateGrid(0, 6);
	m_submissionsGrid->SetColLabelValue(0, "Assignment ID");
	m_submissionsGrid->SetColLabelValue(1, "Accepted");
	m_submissionsGrid->SetColLabelValue(2, "Passed");
	m_submissionsGrid->SetColLabelValue(3, "Tests Passed");
	m_submissionsGrid->SetColLabelValue(4, "Tests Total");
	m_submissionsGrid->SetColLabelValue(5, "Submission Time");
	m_submissionsGrid->AutoSizeColumns();
	m_submissionsGrid->AutoSizeRows();
	m_submissionsGrid->EnableEditing(false);
	m_submissionsGrid->SetSelectionMode(wxGrid::wxGridSelectRows);
	m_submissionsGrid->HideRowLabels();
	m_submissionsGrid->SetGridLineColour(m_submissionsGrid->GetBackgroundColour());
	//m_submittedListBox = new wxListBox(submissionPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, studentSubmissions, wxLB_SINGLE | wxBORDER_SIMPLE);
	wxButton* gradeReportBtn = new wxButton(submissionPanel, wxID_ANY, "Get Grade Report");
	//submissionSizer->Add(m_submittedListBox, 1, wxEXPAND | wxALL, 5);
	submissionSizer->Add(m_submissionsGrid, 1, wxEXPAND | wxALL, 5);
	submissionSizer->Add(gradeReportBtn, 0, wxEXPAND | wxALL, 5);
	submissionPanel->SetSizer(submissionSizer);

	load_assignments();
	get_keys(assignmentMap, studentAssignments);
	load_submissions();
	get_keys(submissionMap, studentSubmissions);


	// Add tabs to notebook
	notebook->AddPage(assignmentPanel, "Assignments");
	notebook->AddPage(submissionPanel, "Submissions");

	mainSizer->Add(notebook, 1, wxEXPAND | wxALL, 5);
	panel->SetSizer(mainSizer);



	// Bind Event Handlers
	m_assignmentsGrid->Bind(wxEVT_GRID_SELECT_CELL, &StudentFrame::OnGridSelect, this);
	submitAssignment->Bind(wxEVT_BUTTON, &StudentFrame::onSubmitAssignment, this);
	incompleteAssignmentsBtn->Bind(wxEVT_BUTTON, &StudentFrame::onGetIncompleteAssignments, this);
	gradeReportBtn->Bind(wxEVT_BUTTON, &StudentFrame::onGetGradeReport, this);

	Layout();
}

void StudentFrame::load_assignments() {
	std::ifstream file("jsonSchemas/assignment-schema.json");

	if (!file.is_open()) {
		wxMessageBox("Failed to open assignment-schema.json", "Error ", wxOK | wxICON_ERROR);
		return;
	}

	json assignments;
	file >> assignments;

	for (const auto& assignment : assignments) {
		auto new_assignment = assignment.get<Assignment>();
		assignmentMap[new_assignment.get_assignment_id()] = std::move(new_assignment);
	}

	file.close();

	if (m_assignmentsGrid) {
		m_assignmentsGrid->ClearGrid();
		if (m_assignmentsGrid->GetNumberRows() > 0) {
			m_assignmentsGrid->DeleteRows(0, m_assignmentsGrid->GetNumberRows());
		}
		m_assignmentsGrid->AppendRows(assignmentMap.size());

		int row = 0;
		for (const auto& pair : assignmentMap) {
			const Assignment& assignment = pair.second;
			m_assignmentsGrid->SetCellValue(row, 0, assignment.get_assignment_id());
			m_assignmentsGrid->SetCellValue(row, 1, assignment.get_due_date());
			row++;
		}
		m_assignmentsGrid->AutoSize();
	}

}

void StudentFrame::load_submissions() {
	std::ifstream file("jsonSchemas/submissions-schema.json");

	if (!file.is_open()) {
		wxMessageBox("Failed to open submissions-schema.json", "Error ", wxOK | wxICON_ERROR);
		return;
	}

	json submissions;
	file >> submissions;

	for (const auto& submission : submissions) {
		if (submission["username"] == currentUser.get_username()) {
			auto new_submission = submission.get<Submission>();
			submissionMap[new_submission.get_id()] = std::move(new_submission);
		}
	}

	file.close();

	if (m_submissionsGrid) {
		m_submissionsGrid->ClearGrid();
		if (m_submissionsGrid->GetNumberRows() > 0) {
			m_submissionsGrid->DeleteRows(0, m_submissionsGrid->GetNumberRows());
		}
		m_submissionsGrid->AppendRows(submissionMap.size());

		int row = 0;
		for (const auto& pair : submissionMap) {
			const Submission& submission = pair.second;
			m_submissionsGrid->SetCellValue(row, 0, submission.get_assignment_id());
			m_submissionsGrid->SetCellValue(row, 1, submission.get_accepted() ? "Yes" : "No");
			m_submissionsGrid->SetCellValue(row, 2, submission.get_passed() ? "Yes" : "No");
			m_submissionsGrid->SetCellValue(row, 3, std::to_string(submission.get_tests_passed()));
			m_submissionsGrid->SetCellValue(row, 4, std::to_string(submission.get_total_tests()));
			m_submissionsGrid->SetCellValue(row, 5, submission.get_submission_time());
			row++;
		}
		m_submissionsGrid->AutoSize();
	}
}

void StudentFrame::onSubmitAssignment(wxCommandEvent& event) {
	int selectedRow = m_assignmentsGrid->GetGridCursorRow();

	if (selectedRow == wxNOT_FOUND) {
		wxMessageBox("Please select an assignment to submit.", "Error", wxOK | wxICON_ERROR);
		return;
	}

	// Get all items from the selected row
	wxString selectedAssignmentName = m_assignmentsGrid->GetCellValue(selectedRow, 0);
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
		"Text files (*.cpp)|*.cpp",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (submissionDialog.ShowModal() == wxID_CANCEL) {
		return;  // User cancelled the file dialog
	}

	// Get the selected file path
	wxString filePath = submissionDialog.GetPath();
	bool isCompiled = false;

	// TODO: Implement file copying logic here
	// For now, we'll just create a submission object

	// Create a new submission object


	// Update or add the submission to the submissionMap

	if (filePath.IsEmpty())
	{
		wxMessageBox("No file selected!", "Error", wxOK | wxICON_ERROR);
		return;
	}

	// Compile the selected .cpp file
	wxString outputFile = "output.exe";
	wxString compileCommand = "g++ \"" + filePath + "\" -o " + outputFile;

	int compileResult = wxExecute(compileCommand, wxEXEC_SYNC);
	if (compileResult != 0)
	{
		wxMessageBox("Compilation failed!", "Error", wxOK | wxICON_ERROR);
		return;
	}

	isCompiled = true;
	int testsPassed = 0;
	// Redirect the process output (stdout and stderr)

	//maybe separate compilation from actual testing

	//loop through assignment tests and run this for each
	const auto& inputs = selectedAssignment.get_inputs();
	const auto& outputs = selectedAssignment.get_outputs();
	const auto& inputBasePath = "inputs/" + selectedAssignment.get_assignment_id() + "/";
	const auto& outputBasePath = "outputs/" + selectedAssignment.get_assignment_id() + "/";

	for (int i = 0; i < inputs.size(); ++i) {
		if (TestExecutable(outputFile, inputBasePath + inputs[i], outputBasePath + outputs[i]))
			++testsPassed;
	}


	// Delete the compiled executable
	remove(outputFile.ToStdString().c_str());
	Submission newSubmission(
		selectedAssignment.get_assignment_id(),
		GenerateGUID(),
		currentUser.get_username(),  // TODO: Replace with actual username
		isCompiled,  // accepted
		testsPassed == inputs.size(),  // passed (to be determined after grading)
		testsPassed,  // tests_passed (to be determined after grading)
		inputs.size(),  // total_tests
		now.FormatISOCombined().ToStdString()  // submission_time
	);
	submissionMap[newSubmission.get_id()] = std::move(newSubmission);

	// Update the submissions list box
	UpdateSubmissionsList();

	// Save the updated submissions to file
	SaveSubmissions();

	wxMessageBox("Assignment submitted successfully!", "Submission", wxOK | wxICON_INFORMATION);
}

bool StudentFrame::TestExecutable(const std::string& execName,
	const std::string& inputPath,
	const std::string& outputPath)
{
	wxProcess process;
	process.Redirect();

	wxString command = "powershell -Command Get-Content ./" + inputPath + " | ./" + execName;
	int executionResult = wxExecute(command, wxEXEC_SYNC, &process);
	if (executionResult == -1)
	{
		wxMessageBox("Failed to execute program!", "Error", wxOK | wxICON_ERROR);
		return false;
	}

	wxInputStream* actualOutputStream = process.GetInputStream();
	wxFileInputStream outputFileStream(outputPath);
	wxInputStream* expectedOutputStream = &outputFileStream;
	if (actualOutputStream && actualOutputStream->IsOk() && expectedOutputStream && expectedOutputStream->IsOk())
	{
		wxTextInputStream inputText(*actualOutputStream);
		wxTextInputStream outputText(*expectedOutputStream);
		wxString output, inputTxt, outputTxt;
		bool flag = true;

		// Read the output line by line and compare with input
		while (!expectedOutputStream->Eof() && !actualOutputStream->Eof()) {
			inputTxt = inputText.ReadLine();
			outputTxt = outputText.ReadLine();
			output += "'" + inputTxt + "' : '" + outputTxt + "'\n";
			if (inputTxt != outputTxt) {
				flag = false;
				wxLogMessage("Comparing input: '%s' with output: '%s'", inputTxt, outputTxt);
			}
		}

		while (!expectedOutputStream->Eof()) {
			outputTxt = outputText.ReadLine().Trim(false).Trim(true);
			if (!outputTxt.IsEmpty()) {
				flag = false;
				wxLogMessage("Expected: '%s' but file ended.", outputTxt);
			}
		}

		while (!actualOutputStream->Eof()) {
			inputTxt = inputText.ReadLine().Trim(false).Trim(true);
			if (!inputTxt.IsEmpty()) {
				flag = false;
				wxLogMessage("Unexpected: '%s' when output ended.", inputTxt);
			}
		}

		return flag;
	}
	return false;
}

void StudentFrame::UpdateSubmissionsList() {
	if (m_submissionsGrid->GetNumberRows() > 0) {
		m_submissionsGrid->DeleteRows(0, m_submissionsGrid->GetNumberRows());
	}

	// Step 2: Re-add rows based on submissionMap size
	m_submissionsGrid->AppendRows(submissionMap.size());

	// Step 3: Populate the grid with updated data
	int row = 0;
	for (const auto& pair : submissionMap) {
		const Submission& submission = pair.second;
		m_submissionsGrid->SetCellValue(row, 0, submission.get_assignment_id());
		m_submissionsGrid->SetCellValue(row, 1, submission.get_accepted() ? "Yes" : "No");
		m_submissionsGrid->SetCellValue(row, 2, submission.get_passed() ? "Yes" : "No");
		m_submissionsGrid->SetCellValue(row, 3, std::to_string(submission.get_tests_passed()));
		m_submissionsGrid->SetCellValue(row, 4, std::to_string(submission.get_total_tests()));
		m_submissionsGrid->SetCellValue(row, 5, submission.get_submission_time());
		row++;
	}

	// Optionally, adjust the columns to fit the data
	m_submissionsGrid->AutoSizeColumns();
	m_submissionsGrid->AutoSizeRows();
	//m_submittedListBox->Set(studentSubmissions);
}

void StudentFrame::SaveSubmissions() {
	json j_submissions = json::array();

	for (const auto& pair : submissionMap) {
		j_submissions.push_back(pair.second);
	}

	std::ofstream file("jsonSchemas/submissions-schema.json");
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

		std::ofstream file("jsonSchemas/assignment-schema.json");
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
		report << std::left << std::setw(20) << submission.get_assignment_id()
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
}

void StudentFrame::OnGridSelect(wxGridEvent& event)
{
	int row = event.GetRow();
	// Do something with the selected row
	event.Skip();
}

void StudentFrame::SetGridColumnWidths(wxGrid* grid, int ncol)
{
	if (grid)
	{
		int gridWidth = grid->GetClientSize().GetWidth();
		int colWidth = gridWidth / ncol;  // Divide by the number of columns (3 in this case)

		for (int i = 0; i < grid->GetNumberCols(); ++i)
		{
			grid->SetColSize(i, colWidth);
		}
	}
}