#pragma once

#include <wx/wx.h>
#include <wx/filepicker.h>
#include <wx/listctrl.h>
#include <vector>
#include <wx/filedlg.h>

class AddAssignmentFrame : public wxDialog
{
public:
    AddAssignmentFrame(wxWindow* parent);

    std::string GetAssignmentID() const;
    std::string GetAssignmentName() const;
    std::string GetDueDate() const;
    std::vector<std::string> GetFiles(bool isInputFiles) const;
    std::vector<std::string> GetInputFiles() const;
    std::vector<std::string> GetOutputFiles() const;

private:
    void OnSubmit(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);

    wxTextCtrl* m_idTextCtrl;
    wxTextCtrl* m_nameTextCtrl;
    wxTextCtrl* m_dueDateTextCtrl;
    wxFilePickerCtrl* m_inputFilesPicker;
    wxFilePickerCtrl* m_outputFilesPicker;

    std::vector<std::string> m_inputFiles;
    std::vector<std::string> m_outputFiles;
};
