#include <wx/wx.h>
#include <wx/filepicker.h>
#include <wx/process.h>
#include <cstdio>
#include <iostream>

class MyFrame : public wxFrame
{
public:
    MyFrame() : wxFrame(nullptr, wxID_ANY, "File Picker and Compiler Example")
    {
        wxPanel* panel = new wxPanel(this);

        // Create the file picker control for .cpp files
        m_filePicker = new wxFilePickerCtrl(panel, wxID_ANY, "", "Choose a .cpp file", "C++ files (*.cpp)|*.cpp");

        // Create a text control to display the selected file path
        m_textCtrl = new wxTextCtrl(panel, wxID_ANY, "");

        // Create the "Compile and Run" button
        m_compileButton = new wxButton(panel, wxID_ANY, "Compile and Run");

        // Bind the events to handler functions
        Bind(wxEVT_FILEPICKER_CHANGED, &MyFrame::OnFilePicked, this);
        Bind(wxEVT_BUTTON, &MyFrame::OnCompileAndRun, this, m_compileButton->GetId());

        // Add the widgets to the sizer
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(m_filePicker, 0, wxEXPAND | wxALL, 5);
        sizer->Add(m_textCtrl, 0, wxEXPAND | wxALL, 5);
        sizer->Add(m_compileButton, 0, wxALL | wxCENTER, 5);
        panel->SetSizer(sizer);
    }

private:
    void OnFilePicked(wxFileDirPickerEvent& event)
    {
        // Get the selected file path and display it in the text control
        wxString filePath = m_filePicker->GetPath();
        m_textCtrl->SetValue(filePath);
    }

    void OnCompileAndRun(wxCommandEvent& event)
    {
        wxString filePath = m_textCtrl->GetValue();

        if (filePath.IsEmpty())
        {
            wxMessageBox("No file selected!", "Error", wxOK | wxICON_ERROR);
            return;
        }

        // Compile the selected .cpp file
        wxString outputFile = "output.exe";
        wxString command = "g++ \"" + filePath + "\" -o " + outputFile;

        int compileResult = wxExecute(command, wxEXEC_SYNC);
        if (compileResult != 0)
        {
            wxMessageBox("Compilation failed!", "Error", wxOK | wxICON_ERROR);
            return;
        }

        // Run the compiled program
        wxExecute(outputFile, wxEXEC_SYNC);
        int status = remove(outputFile.ToStdString().c_str());

        if (status != 0)
        {
            wxMessageBox(outputFile.ToStdString().c_str(), "Error", wxOK | wxICON_ERROR);
            return;
        }
    }

    wxFilePickerCtrl* m_filePicker;
    wxTextCtrl* m_textCtrl;
    wxButton* m_compileButton;
};

class MyApp : public wxApp
{
public:
    virtual bool OnInit()
    {
        MyFrame* frame = new MyFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
