#include "AddAssignmentFrame.h"



AddAssignmentFrame::AddAssignmentFrame(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Add Assignment", wxDefaultPosition, wxSize(400, 300))
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* idSizer = new wxBoxSizer(wxHORIZONTAL);
    idSizer->Add(new wxStaticText(this, wxID_ANY, "Assignment ID:"), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    m_idTextCtrl = new wxTextCtrl(this, wxID_ANY);
    idSizer->Add(m_idTextCtrl, 1, wxALL | wxEXPAND, 5);

    wxBoxSizer* nameSizer = new wxBoxSizer(wxHORIZONTAL);
    nameSizer->Add(new wxStaticText(this, wxID_ANY, "Assignment Name:"), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    m_nameTextCtrl = new wxTextCtrl(this, wxID_ANY);
    nameSizer->Add(m_nameTextCtrl, 1, wxALL | wxEXPAND, 5);

    wxBoxSizer* dueDateSizer = new wxBoxSizer(wxHORIZONTAL);
    dueDateSizer->Add(new wxStaticText(this, wxID_ANY, "Due Date:"), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    m_dueDateTextCtrl = new wxTextCtrl(this, wxID_ANY);
    dueDateSizer->Add(m_dueDateTextCtrl, 1, wxALL | wxEXPAND, 5);

    wxBoxSizer* inputSizer = new wxBoxSizer(wxHORIZONTAL);
    inputSizer->Add(new wxStaticText(this, wxID_ANY, "Input Files:"), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    m_inputFilesPicker = new wxFilePickerCtrl(this, wxID_ANY, "", "Choose files", "All files (*.*)|*.*", wxDefaultPosition, wxDefaultSize, wxFLP_FILE_MUST_EXIST | wxFLP_USE_TEXTCTRL | wxFLP_MULTIPLE);
    inputSizer->Add(m_inputFilesPicker, 1, wxALL | wxEXPAND, 5);

    wxBoxSizer* outputSizer = new wxBoxSizer(wxHORIZONTAL);
    outputSizer->Add(new wxStaticText(this, wxID_ANY, "Output Files:"), 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    m_outputFilesPicker = new wxFilePickerCtrl(this, wxID_ANY, "", "Choose files", "All files (*.*)|*.*", wxDefaultPosition, wxDefaultSize, wxFLP_FILE_MUST_EXIST | wxFLP_USE_TEXTCTRL | wxFLP_MULTIPLE);
    outputSizer->Add(m_outputFilesPicker, 1, wxALL | wxEXPAND, 5);

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* submitButton = new wxButton(this, wxID_OK, "Submit");
    wxButton* cancelButton = new wxButton(this, wxID_CANCEL, "Cancel");
    buttonSizer->Add(submitButton, 0, wxALL | wxALIGN_CENTER, 5);
    buttonSizer->Add(cancelButton, 0, wxALL | wxALIGN_CENTER, 5);

    sizer->Add(idSizer, 0, wxEXPAND);
    sizer->Add(nameSizer, 0, wxEXPAND);
    sizer->Add(dueDateSizer, 0, wxEXPAND);
    sizer->Add(inputSizer, 0, wxEXPAND);
    sizer->Add(outputSizer, 0, wxEXPAND);
    sizer->Add(buttonSizer, 0, wxALIGN_CENTER);

    SetSizerAndFit(sizer);

    Bind(wxEVT_BUTTON, &AddAssignmentFrame::OnSubmit, this, wxID_OK);
    Bind(wxEVT_BUTTON, &AddAssignmentFrame::OnCancel, this, wxID_CANCEL);
}

std::string AddAssignmentFrame::GetAssignmentID() const {
    return m_idTextCtrl->GetValue().ToStdString();
}

std::string AddAssignmentFrame::GetAssignmentName() const {
    return m_nameTextCtrl->GetValue().ToStdString();
}

std::string AddAssignmentFrame::GetDueDate() const {
    return m_dueDateTextCtrl->GetValue().ToStdString();
}

std::vector<std::string> AddAssignmentFrame::GetFiles(bool isInputFiles) const {
    wxString title = isInputFiles ? "Choose input files" : "Choose output files";
    wxString wildcard = "All files (*.*)|*.*";
    wxFileDialog fileDialog(this, title, "", "", wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);

    if (fileDialog.ShowModal() == wxID_CANCEL)
        return {};

    wxArrayString files = fileDialog.GetPaths();
    std::vector<std::string> fileVec(files.GetCount());
    for (size_t i = 0; i < files.GetCount(); ++i) {
        fileVec[i] = files[i].ToStdString();
    }
    return fileVec;
}

std::vector<std::string> AddAssignmentFrame::GetInputFiles() const {
    return GetFiles(true);
}

std::vector<std::string> AddAssignmentFrame::GetOutputFiles() const {
    return GetFiles(false);
}

void AddAssignmentFrame::OnSubmit(wxCommandEvent& event) {
    // Perform validation and processing
    if (GetInputFiles().size() != GetOutputFiles().size()) {
        wxMessageBox("The number of input files must match the number of output files.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    EndModal(wxID_OK);
}

void AddAssignmentFrame::OnCancel(wxCommandEvent& event) {
    EndModal(wxID_CANCEL);
}
