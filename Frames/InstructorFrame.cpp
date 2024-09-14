#include "../Header Files/InstructorFrame.h"

InstructorFrame::InstructorFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, "Instructor Dashboard", wxDefaultPosition, wxSize(500, 400))
{
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    Bind(wxEVT_CLOSE_WINDOW, &InstructorFrame::OnClose, this);

    // Create notebook (tabbed interface)
    wxNotebook* notebook = new wxNotebook(panel, wxID_ANY);

    // Create first tab for students
    wxPanel* studentsPanel = new wxPanel(notebook);
    wxBoxSizer* studentsSizer = new wxBoxSizer(wxVERTICAL);
    m_studentsListBox = new wxListBox(studentsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, studentUsernames, wxLB_SINGLE | wxBORDER_SIMPLE);
    studentsSizer->Add(m_studentsListBox, 1, wxEXPAND | wxALL, 5);
    studentsPanel->SetSizer(studentsSizer);

    // Create second tab for assignments
    wxPanel* assignmentsPanel = new wxPanel(notebook);
    wxBoxSizer* assignmentsSizer = new wxBoxSizer(wxVERTICAL);
    m_assignmentsGrid = new wxGrid(assignmentsPanel, wxID_ANY);
    m_assignmentsGrid->CreateGrid(0, 2);
    m_assignmentsGrid->SetColLabelValue(0, "Assignment ID");
    m_assignmentsGrid->SetColLabelValue(1, "Due Date");
    m_assignmentsGrid->AutoSizeColumns();
    m_assignmentsGrid->AutoSizeRows();
    m_assignmentsGrid->EnableEditing(false);
    m_assignmentsGrid->SetSelectionMode(wxGrid::wxGridSelectRows);
    m_assignmentsGrid->HideRowLabels();
    m_assignmentsGrid->SetGridLineColour(m_assignmentsGrid->GetBackgroundColour());
    //m_assignmentsListBox = new wxListBox(assignmentsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, assignmentsArray, wxLB_SINGLE | wxBORDER_SIMPLE);
    wxButton* addAssignmentBtn = new wxButton(assignmentsPanel, wxID_ANY, "Add Assignment");
    wxButton* removeAssignmentBtn = new wxButton(assignmentsPanel, wxID_ANY, "Delete Assignment");
    wxButton* assignmentReportBtn = new wxButton(assignmentsPanel, wxID_ANY, "Get Assignment Report");

    assignmentsSizer->Add(m_assignmentsGrid, 1, wxEXPAND | wxALL, 5);
    //assignmentsSizer->Add(m_assignmentsListBox, 1, wxEXPAND | wxALL, 5);
    assignmentsSizer->Add(addAssignmentBtn, 0, wxEXPAND | wxALL, 5);
    assignmentsSizer->Add(removeAssignmentBtn, 0, wxEXPAND | wxALL, 5);
    assignmentsSizer->Add(assignmentReportBtn, 0, wxEXPAND | wxALL, 5);
    assignmentsPanel->SetSizer(assignmentsSizer);

    // Add tabs to notebook
    notebook->AddPage(studentsPanel, "Students");
    notebook->AddPage(assignmentsPanel, "Assignments");

    // Add notebook to main sizer
    mainSizer->Add(notebook, 1, wxEXPAND | wxALL, 5);

    panel->SetSizer(mainSizer);

    load_students();
    get_keys(studentsMap, studentUsernames);

    load_assignments();
    get_keys(assignmentsMap, assignmentsArray);

    // Bind event handlers
    addAssignmentBtn->Bind(wxEVT_BUTTON, &InstructorFrame::OnAddAssignment, this);
    removeAssignmentBtn->Bind(wxEVT_BUTTON, &InstructorFrame::OnDeleteAssignment, this);
    assignmentReportBtn->Bind(wxEVT_BUTTON, &InstructorFrame::OnGetAssignmentReport, this);
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

void InstructorFrame::load_assignments()
{
    std::ifstream file("jsonSchemas/assignment-schema.json");

    if (!file.is_open()) {
        wxMessageBox("Failed to open users-schema.json", "Error ", wxOK | wxICON_ERROR);
        return;
    }

    json assignments;
    file >> assignments;

    for (const auto& assignment : assignments) {
        auto new_assignment = assignment.get<Assignment>();
        assignmentsMap[new_assignment.get_assignment_id()] = std::move(new_assignment);
    }

    file.close();

    UpdateAssignmentssList();
}

void InstructorFrame::OnAddAssignment(wxCommandEvent& event) {
    // Create the AddAssignmentFrame as a child of this frame
    AddAssignmentFrame* assignmentFrame = new AddAssignmentFrame(this);

    // Show the frame as a modal dialog
    if (assignmentFrame->ShowModal() == wxID_OK) {
        // Retrieve the data from the dialog
        std::string assignmentID = assignmentFrame->GetAssignmentID();
        std::string dueDate = assignmentFrame->GetDueDate();
        std::vector<std::string> inputFiles = assignmentFrame->GetInputFiles();
        std::vector<std::string> outputFiles = assignmentFrame->GetOutputFiles();

        std::filesystem::path inputDestination = std::filesystem::current_path() / "inputs" / assignmentID;
        for (auto& input : inputFiles) {
            std::filesystem::path sourceFile = input;
            auto target = inputDestination / sourceFile.filename();

            try // If you want to avoid exception handling, then use the error code overload of the following functions.
            {
                std::filesystem::create_directories(inputDestination); // Recursively create target directory if not existing.
                std::filesystem::copy_file(sourceFile, target, std::filesystem::copy_options::overwrite_existing);
                input = sourceFile.filename().generic_string();
            }
            catch (std::exception& e) // Not using fs::filesystem_error since std::bad_alloc can throw too.  
            {
                std::cout << e.what();
            }

        }

        std::filesystem::path outputDestination = std::filesystem::current_path() / "outputs" / assignmentID;
        for (auto& input : outputFiles) {
            std::filesystem::path sourceFile = input;
            auto target = outputDestination / sourceFile.filename();

            try // If you want to avoid exception handling, then use the error code overload of the following functions.
            {
                std::filesystem::create_directories(outputDestination); // Recursively create target directory if not existing.
                std::filesystem::copy_file(sourceFile, target, std::filesystem::copy_options::overwrite_existing);
                input = sourceFile.filename().generic_string();
            }
            catch (std::exception& e) // Not using fs::filesystem_error since std::bad_alloc can throw too.  
            {
                std::cout << e.what();
            }

        }

        assignmentsMap[assignmentID] = Assignment(assignmentID, dueDate, inputFiles, outputFiles, true);

        assignmentsArray.Add(assignmentID);
        //m_assignmentsListBox->Set(assignmentsArray);

        UpdateAssignmentssList();
    }

    // Destroy the dialog to free resources
    assignmentFrame->Destroy();
}

void InstructorFrame::OnClose(wxCloseEvent& event)
{
    // Convert the usersMap (or your data structure) back to JSON

    if (assignmentsMap.empty()) {
        event.Skip();
        return;
    }

    json j_users = json::array();

    // Iterate over the map and add each user object to the array
    for (const auto& pair : assignmentsMap) {
        j_users.push_back(pair.second);

        // Write the JSON to the file
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

void InstructorFrame::OnGridSelect(wxGridEvent& event)
{
    int row = event.GetRow();
    // Do something with the selected row
    event.Skip();
}

void InstructorFrame::OnGetAssignmentReport(wxCommandEvent& event)
{
    // Get the selected assignment
    int selectedRow = m_assignmentsGrid->GetGridCursorRow();
    if (selectedRow == wxNOT_FOUND) {
        wxMessageBox("Please select an assignment to generate a report.", "Error", wxOK | wxICON_ERROR);
        return;
    }
    wxString selectedAssignmentId = m_assignmentsGrid->GetCellValue(selectedRow, 0);

    // Load submissions
    std::vector<Submission> submissions;
    load_submissions(submissions);

    // Create a report dialog
    wxDialog* reportDialog = new wxDialog(this, wxID_ANY, "Assignment Report: " + selectedAssignmentId, wxDefaultPosition, wxSize(600, 400));
    wxTextCtrl* reportText = new wxTextCtrl(reportDialog, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

    wxString report;
    report += "Assignment Report for: " + selectedAssignmentId + "\n\n";

    // Go through all students
    for (const auto& student : studentsMap) {
        wxString studentUsername = student.first;
        report += "Student: " + studentUsername + "\n";

        // Find the latest submission for this student and assignment
        const Submission* latestSubmission = nullptr;
        for (const auto& submission : submissions) {
            if (submission.get_assignment_id() == selectedAssignmentId.ToStdString() &&
                submission.get_username() == studentUsername.ToStdString()) {
                if (!latestSubmission || submission.get_submission_time() > latestSubmission->get_submission_time()) {
                    latestSubmission = &submission;
                }
            }
        }

        if (latestSubmission) {
            report += "Status: Submitted\n";
            report += "Submission Time: " + wxString(latestSubmission->get_submission_time()) + "\n";
            report += "Passed: " + wxString(latestSubmission->get_passed() ? "Yes" : "No") + "\n";
            report += "Tests Passed: " + wxString::Format("%d/%d", latestSubmission->get_tests_passed(), latestSubmission->get_total_tests()) + "\n";

            // Calculate grade (assuming grade is based on percentage of tests passed)
            double grade = (static_cast<double>(latestSubmission->get_tests_passed()) / latestSubmission->get_total_tests()) * 100;
            report += wxString::Format("Grade: %.2f%%\n", grade);
        }
        else {
            report += "Status: Not Submitted\n";
            report += "Grade: 0%\n";
        }

        report += "\n";  // Add a blank line between students
    }

    reportText->SetValue(report);

    // Layout
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(reportText, 1, wxEXPAND | wxALL, 10);
    reportDialog->SetSizer(sizer);

    // Show the dialog
    reportDialog->ShowModal();
    reportDialog->Destroy();
}

void InstructorFrame::load_submissions(std::vector<Submission>& submissions)
{
    std::ifstream file("jsonSchemas/submissions-schema.json");
    if (!file.is_open()) {
        wxMessageBox("Failed to open submission-schema.json", "Error", wxOK | wxICON_ERROR);
        return;
    }

    json submissionsJson;
    file >> submissionsJson;

    for (const auto& submissionJson : submissionsJson) {
        submissions.push_back(submissionJson.get<Submission>());
    }

    file.close();
}

void InstructorFrame::UpdateAssignmentssList() {
    if (m_assignmentsGrid->GetNumberRows() > 0) {
        m_assignmentsGrid->DeleteRows(0, m_assignmentsGrid->GetNumberRows());
    }

    // Step 3: Populate the grid with updated data
    int row = 0;
    for (const auto& pair : assignmentsMap) {
        if (pair.second.isActive()) {
            m_assignmentsGrid->AppendRows();
            const Assignment& assignment = pair.second;
            m_assignmentsGrid->SetCellValue(row, 0, assignment.get_assignment_id());
            m_assignmentsGrid->SetCellValue(row, 1, assignment.get_due_date());
            row++;
        }
    }

    // Optionally, adjust the columns to fit the data
    m_assignmentsGrid->AutoSizeColumns();
    m_assignmentsGrid->AutoSizeRows();
    //m_submittedListBox->Set(studentSubmissions);
}

void InstructorFrame::OnDeleteAssignment(wxCommandEvent& event)
{
    int selectedRow = m_assignmentsGrid->GetGridCursorRow();

    if (selectedRow == wxNOT_FOUND) {
        wxMessageBox("Please select an assignment to submit.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    wxString selectedAssignmentName = m_assignmentsGrid->GetCellValue(selectedRow, 0);
    wxMessageDialog confirmDlg(this, "Are you sure you want to delete the assignment '" + selectedAssignmentName + "'?", "Confirm Deletion", wxYES_NO | wxNO_DEFAULT | wxICON_WARNING);
    if (confirmDlg.ShowModal() == wxID_YES) {
        assignmentsMap[selectedAssignmentName.ToStdString()].deleteAssignment();
    }

    UpdateAssignmentssList();
}