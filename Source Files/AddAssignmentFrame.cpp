#include "../Header Files/AddAssignmentFrame.h"

AddAssignmentFrame::AddAssignmentFrame(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Add Assignment", wxDefaultPosition, wxSize(400, 300))
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // Assignment ID Input
    wxBoxSizer* idSizer = new wxBoxSizer(wxHORIZONTAL);
    idSizer->Add(new wxStaticText(this, wxID_ANY, "Assignment ID:"), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    m_idTextCtrl = new wxTextCtrl(this, wxID_ANY);
    idSizer->Add(m_idTextCtrl, 1, wxALL | wxEXPAND, 5);

    // Due Date Input
    wxBoxSizer* dueDateSizer = new wxBoxSizer(wxHORIZONTAL);
    dueDateSizer->Add(new wxStaticText(this, wxID_ANY, "Due Date:"), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    m_dueDatePicker = new wxDatePickerCtrl(this, wxID_ANY);
    m_dueTimePicker = new wxTimePickerCtrl(this, wxID_ANY);
    dueDateSizer->Add(m_dueDatePicker, 1, wxALL | wxEXPAND, 5);
    dueDateSizer->Add(m_dueTimePicker, 1, wxALL | wxEXPAND, 5);

    // Buttons
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* submitButton = new wxButton(this, wxID_OK, "Submit");
    wxButton* cancelButton = new wxButton(this, wxID_CANCEL, "Cancel");
    buttonSizer->Add(submitButton, 0, wxALL | wxALIGN_CENTER, 5);
    buttonSizer->Add(cancelButton, 0, wxALL | wxALIGN_CENTER, 5);

    // Add sizers to main sizer
    sizer->Add(idSizer, 0, wxEXPAND);
    sizer->Add(dueDateSizer, 0, wxEXPAND);
    sizer->Add(buttonSizer, 0, wxALIGN_CENTER);

    SetSizerAndFit(sizer);

    // Bind events
    Bind(wxEVT_BUTTON, &AddAssignmentFrame::OnSubmit, this, wxID_OK);
    Bind(wxEVT_BUTTON, &AddAssignmentFrame::OnCancel, this, wxID_CANCEL);
}

std::string AddAssignmentFrame::GetAssignmentID() const {
    return m_idTextCtrl->GetValue().ToStdString();
}

std::string AddAssignmentFrame::GetDueDate() const {
    wxDateTime dueDateTime = m_dueDatePicker->GetValue();
    int hour = m_dueTimePicker->GetValue().GetHour();
    int minute = m_dueTimePicker->GetValue().GetMinute();
    dueDateTime.SetHour(hour);
    dueDateTime.SetMinute(minute);
    return dueDateTime.FormatISOCombined().ToStdString();
}

std::vector<std::string> AddAssignmentFrame::GetInputFiles() const {
    return m_inputFiles;
}

std::vector<std::string> AddAssignmentFrame::GetOutputFiles() const {
    return m_outputFiles;
}

void AddAssignmentFrame::OnSubmit(wxCommandEvent& event) {
    // Clear the internal file lists
    m_inputFiles.clear();
    m_outputFiles.clear();

    // Open file dialog for selecting input files
    wxFileDialog inputFileDialog(this, "Choose Input Files", "", "",
        "Text files (*.txt)|*.txt",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
    if (inputFileDialog.ShowModal() == wxID_OK) {
        wxArrayString inputFilesArray;
        inputFileDialog.GetPaths(inputFilesArray);
        for (const auto& filePath : inputFilesArray) {
            m_inputFiles.push_back(filePath.ToStdString());
        }
    }
    else {
        // User cancelled the file dialog
        return;
    }

    // Open file dialog for selecting output files
    wxFileDialog outputFileDialog(this, "Choose Output Files", "", "",
        "Text files (*.txt)|*.txt",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
    if (outputFileDialog.ShowModal() == wxID_OK) {
        wxArrayString outputFilesArray;
        outputFileDialog.GetPaths(outputFilesArray);
        for (const auto& filePath : outputFilesArray) {
            m_outputFiles.push_back(filePath.ToStdString());
        }
    }
    else {
        // User cancelled the file dialog
        return;
    }

    // Validate Assignment ID
    if (m_idTextCtrl->GetValue().IsEmpty()) {
        wxMessageBox("Assignment ID cannot be empty.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Validate Due Date
    if (!m_dueDatePicker->GetValue().IsValid()) {
        wxMessageBox("Due Date cannot be empty or invalid.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Validate the number of input and output files
    if (m_inputFiles.size() != m_outputFiles.size()) {
        wxMessageBox("The number of input files must match the number of output files.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // If all validations pass, close the dialog with OK status
    EndModal(wxID_OK);
}

void AddAssignmentFrame::OnCancel(wxCommandEvent& event) {
    EndModal(wxID_CANCEL);
}