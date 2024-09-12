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
#include "User.h"
#include "Assignment.h"
#include "Submission.h"
#include "Util.h"
#include "AddAssignmentFrame.h"


using json = nlohmann::json;

class StudentFrame : public wxFrame {
public:
	StudentFrame(const wxString& title, User studentUser);

private:
	User currentUser;
	wxListBox* m_submittedListBox;
	wxListBox* m_assignmentsListBox;
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
	void UpdateSubmissionsGrid();

	void onGetGradeReport(wxCommandEvent& event);
	void onGetIncompleteAssignments(wxCommandEvent& event);
	void OnGridSelect(wxGridEvent& event);

	void SetGridColumnWidths(wxGrid* grid, int ncol = 2);

};