#pragma once
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
#include "User.h"
#include "Assignment.h"
#include "Submission.h"
#include "Util.h"
#include "AddAssignmentFrame.h"


using json = nlohmann::json;

class StudentFrame : public wxFrame {
public:
	StudentFrame(const wxString& title);

private:
	wxListBox* m_submittedListBox;
	wxListBox* m_assignmentsListBox;
	wxArrayString studentAssignments;
	wxArrayString studentSubmissions;
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

};