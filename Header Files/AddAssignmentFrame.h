#pragma once
#include <wx/wx.h>
#include <wx/filepicker.h>
#include <wx/listctrl.h>
#include <vector>
#include <wx/datectrl.h>
#include <wx/timectrl.h>
#include <wx/filedlg.h>

class AddAssignmentFrame : public wxDialog
{
public:
    AddAssignmentFrame(wxWindow* parent);

    std::string GetAssignmentID() const;
    std::string GetDueDate() const;
    std::vector<std::string> GetInputFiles() const;
    std::vector<std::string> GetOutputFiles() const;

private:
    void OnSubmit(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);

    wxTextCtrl* m_idTextCtrl;
    wxDatePickerCtrl* m_dueDatePicker;
    wxTimePickerCtrl* m_dueTimePicker;

    std::vector<std::string> m_inputFiles;
    std::vector<std::string> m_outputFiles;
};