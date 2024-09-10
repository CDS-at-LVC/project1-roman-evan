#pragma once
#pragma once

#include <wx/wx.h>
#include <wx/filepicker.h>
#include <wx/process.h>
#include <wx/wfstream.h> 
#include <cstdio>
#include <iostream>
#include <fstream>
#include <wx/txtstrm.h>
#include <sstream>
#include <algorithm>


class MyFrame : public wxFrame
{
public:
	MyFrame() : wxFrame(nullptr, wxID_ANY, "File Picker and Compiler Example")
	{
		wxPanel* panel = new wxPanel(this);

		// Create the file picker control for .cpp files
		m_filePicker = new wxFilePickerCtrl(panel, wxID_ANY, "", "Choose a .cpp file", "C++ files (*.cpp)|*.cpp");

		// Create the "Compile and Run" button
		m_compileButton = new wxButton(panel, wxID_ANY, "Compile and Run");

		// Bind the events to handler functions
		Bind(wxEVT_FILEPICKER_CHANGED, &MyFrame::OnFilePicked, this);
		Bind(wxEVT_BUTTON, &MyFrame::OnCompileAndRun, this, m_compileButton->GetId());

		// Add the widgets to the sizer
		wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
		sizer->Add(m_filePicker, 0, wxEXPAND | wxALL, 5);
		sizer->Add(m_compileButton, 0, wxALL | wxCENTER, 5);
		panel->SetSizer(sizer);
	}

private:

	void OnFilePicked(wxFileDirPickerEvent& event) {

	}

	bool TestExecutable(const std::string& execName,
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


	}

	void OnCompileAndRun(wxCommandEvent& event)
	{
		wxString filePath = m_filePicker->GetPath();

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

		// Set up a wxProcess for capturing output
		wxProcess process;
		process.Redirect();  // Redirect the process output (stdout and stderr)

		//maybe separate compilation from actual testing

		//loop through assignment tests and run this for each
		TestExecutable(outputFile, "inputs/input1.txt", "outputs/output1.txt");

		// Delete the compiled executable
		remove(outputFile.ToStdString().c_str());
	}

	wxFilePickerCtrl* m_filePicker;
	wxButton* m_compileButton;
};