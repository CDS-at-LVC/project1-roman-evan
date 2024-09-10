#include "../Header Files/InstructorFrame.h"

InstructorFrame::InstructorFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, "Instructor Dashboard", wxDefaultPosition, wxSize(500, 400))
{
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    Bind(wxEVT_CLOSE_WINDOW, &InstructorFrame::OnClose, this);

    load_students();
    get_keys(studentsMap, studentUsernames);

    load_assignments();
    get_keys(assignmentsMap, assignmentsArray);

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
    m_assignmentsListBox = new wxListBox(assignmentsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, assignmentsArray, wxLB_SINGLE | wxBORDER_SIMPLE);
    wxButton* addAssignmentBtn = new wxButton(assignmentsPanel, wxID_ANY, "Add Assignment");
    assignmentsSizer->Add(m_assignmentsListBox, 1, wxEXPAND | wxALL, 5);
    assignmentsSizer->Add(addAssignmentBtn, 0, wxEXPAND | wxALL, 5);
    assignmentsPanel->SetSizer(assignmentsSizer);

    // Add tabs to notebook
    notebook->AddPage(studentsPanel, "Students");
    notebook->AddPage(assignmentsPanel, "Assignments");

    // Add notebook to main sizer
    mainSizer->Add(notebook, 1, wxEXPAND | wxALL, 5);

    panel->SetSizer(mainSizer);

    // Bind event handlers
    //refreshBtn->Bind(wxEVT_BUTTON, &InstructorFrame::OnRefresh, this);
    addAssignmentBtn->Bind(wxEVT_BUTTON, &InstructorFrame::OnAddAssignment, this);
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

void InstructorFrame::load_assignments()
{
    std::ifstream file("assignment-schema.json");

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


        assignmentsMap[assignmentID] = Assignment(assignmentID, dueDate, inputFiles, outputFiles);

        assignmentsArray.Add(assignmentID);
        m_assignmentsListBox->Set(assignmentsArray);
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