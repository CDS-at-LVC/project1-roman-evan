#pragma once
#include <wx/listctrl.h>
#include <wx/wx.h>
#include <fstream>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <vector>
#include <wx/datectrl.h>
#include <wx/aui/auibook.h>
#include <filesystem>
#include <iostream>
#include <wx/gbsizer.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/grid.h>
#include <wx/datetime.h>
#include <sstream>
#include <iomanip>
#include <wx/process.h>
#include <wx/wfstream.h> 
#include <cstdio>
#include <wx/txtstrm.h>
#include <algorithm>
#include "../Models/User.h"
#include "../Models/Assignment.h"
#include "../Models/Submission.h"
#include "../Util/Util.h"
#include "AddAssignmentFrame.h"



using json = nlohmann::json;

class StudentFrame : public wxFrame {
public:
	StudentFrame(const wxString& title, User studentUser);

private:
	User currentUser;
	//wxListBox* m_submittedListBox;
	//wxListBox* m_assignmentsListBox;
	wxArrayString studentAssignments;
	wxArrayString studentSubmissions;
	wxGrid* m_assignmentsGrid;
	wxGrid* m_submissionsGrid;
	std::unordered_map<wxString, Assignment> assignmentMap;
	std::unordered_map<wxString, Submission> submissionMap;

	void load_assignments();
	void load_submissions();

	void onSubmitAssignment(wxCommandEvent& event);
	void onClose(wxCloseEvent& event);

	void UpdateSubmissionsList();
	void SaveSubmissions();
	//void onRefresh(wxCloseEvent& event);

	void onGetGradeReport(wxCommandEvent& event);
	void onGetIncompleteAssignments(wxCommandEvent& event);
	void OnGridSelect(wxGridEvent& event);
	bool TestExecutable(const std::string& execName,
		const std::string& inputPath,
		const std::string& outputPath);


	void SetGridColumnWidths(wxGrid* grid, int ncol = 2);

};