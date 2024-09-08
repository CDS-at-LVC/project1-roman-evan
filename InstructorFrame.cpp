#include "InstructorFrame.h"
#include <wx/wx.h>
#include <wx/filepicker.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <ctime>
#include <wx/msgdlg.h>
#define _CRT_SECURE_NO_WARNINGS


using json = nlohmann::json;

InstructorFrame::InstructorFrame(const wxString& title, User current_user) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(400, 300)), current_user(current_user)
{
	wxPanel* instructor_panel = new wxPanel(this, wxID_ANY);
	instructor_panel->SetBackgroundColour(wxColour(240, 240, 240));

	wxButton* create_assignment_button = new wxButton(instructor_panel, wxID_ANY, "Create User", wxDefaultPosition, wxSize(100, 30));
	wxButton* delete_assignment_button = new wxButton(instructor_panel, wxID_ANY, "Delete User", wxDefaultPosition, wxSize(100, 30));

	// Bind events to the buttons
	create_assignment_button->Bind(wxEVT_BUTTON, &InstructorFrame::on_create_assignment, this);
	delete_assignment_button->Bind(wxEVT_BUTTON, &InstructorFrame::on_delete_assignment, this);

	// List for assignments
	wxArrayString wx_assignment_array;
	update_assignments();
	for (const auto& assignment : assignment_vector) {
		wx_assignment_array.Add(wxString::FromUTF8(assignment.c_str())); // Safe string conversion
	}

	assignment_list = new wxListBox(instructor_panel, wxID_ANY, wxDefaultPosition, wxSize(250, 150), wx_assignment_array, wxLB_SINGLE | wxBORDER_SIMPLE);
	assignment_list->Bind(wxEVT_LISTBOX, &InstructorFrame::on_select_assignment, this);

	// List for students
	wxArrayString wx_student_array;
	student_list = new wxListBox(instructor_panel, wxID_ANY, wxDefaultPosition, wxSize(250, 150), wx_student_array, wxLB_SINGLE | wxBORDER_SIMPLE);

	student_list = new wxListBox(instructor_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wx_student_array, wxLB_SINGLE | wxBORDER_SIMPLE);
	student_list->SetBackgroundColour(wxColour(255, 255, 255));  // White background for the list
	student_list->SetForegroundColour(wxColour(0, 0, 0));        // Black text color
	student_list->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	wxBoxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);
	button_sizer->Add(create_assignment_button, 0, wxRIGHT, 10);
	button_sizer->Add(delete_assignment_button, 0);

	wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
	main_sizer->AddSpacer(20); // Add space at the top
	main_sizer->Add(button_sizer, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 20); // Align buttons to the right
	main_sizer->Add(student_list, 1, wxALL | wxEXPAND, 20);  // Add the list with padding

	instructor_panel->SetSizer(main_sizer);

	Centre();
}

std::vector<std::string> InstructorFrame::get_students() {
	student_vector.clear();
	std::ifstream file("users-schema.json");

	if (!file.is_open()) {
		wxMessageBox("Failed to open users-schema.json", "Error", wxOK | wxICON_ERROR);
		return student_vector;
	}

	json users;
	file >> users;

	for (const auto& user : users) {
		if (user["role"] == "student") {
			std::string username = user["username"].get<std::string>();
			student_vector.push_back(username);  // Safe string extraction
		}
	}

	file.close();
	return student_vector;
}

void InstructorFrame::create_assignment(std::string assignment_name, std::vector<std::string> input_files, std::vector<std::string> output_files, time_t due_date) {
	std::ifstream file("assignment-schema2.json");
	if (!file.is_open()) {
		wxMessageBox("Failed to open assignments-schema2.json", "Error", wxOK | wxICON_ERROR);
		return;
	}

	json assignments;
	file >> assignments;
	file.close();

	for (const auto& assignment : assignments) {
		if (assignment["assignment_name"] == assignment_name) {
			wxMessageBox("Assignment already exists!", "Error", wxOK | wxICON_ERROR);
			return;
		}
	}

	json new_assignment = {
		{"assignment_name", assignment_name},
		{"input_files", input_files},
		{"output_files", output_files},
		{"due_date", due_date}
	};

	assignments.push_back(new_assignment);

	std::ofstream out_file("assignments-schema2.json");
	out_file << assignments.dump(4); // Pretty-print with indentation
	out_file.close();

	update_assignments();
}

void InstructorFrame::delete_assignment(std::string assignment_name) {
	std::ifstream file("assignment-schema2.json");
	if (!file.is_open()) {
		wxMessageBox("Failed to open assignments-schema2.json", "Error", wxOK | wxICON_ERROR);
		return;
	}

	json assignments;
	file >> assignments;
	file.close();

	auto it = std::remove_if(assignments.begin(), assignments.end(), [&](const json& assignment) {
		return assignment["assignment_name"] == assignment_name;
		});

	if (it != assignments.end()) {
		assignments.erase(it, assignments.end());
		std::ofstream out_file("assignment-schema2.json");
		out_file << assignments.dump(4);
		out_file.close();
		update_assignments();
	}
	else {
		wxMessageBox("Assignment not found!", "Error", wxOK | wxICON_ERROR);
	}
}

void InstructorFrame::on_create_assignment(wxCommandEvent& event) {
	wxTextEntryDialog dialog(this, "Enter assignment name", "Create Assignment");
	if (dialog.ShowModal() == wxID_OK) {
		std::string assignment_name = dialog.GetValue().ToStdString();
		create_assignment(assignment_name, {}, {}, std::time(nullptr));
	}
}

void InstructorFrame::on_delete_assignment(wxCommandEvent& event) {
	wxString selected_assignment = assignment_list->GetStringSelection();
	if (!selected_assignment.IsEmpty()) {
		delete_assignment(selected_assignment.ToStdString());
	}
}

void InstructorFrame::update_assignments() {
	//assignment_vector.clear();
	//assignment_list->Clear();

	std::ifstream file("assignment-schema2.json");
	if (!file.is_open()) {
		wxMessageBox("Failed to open assignments-schema2.json", "Error", wxOK | wxICON_ERROR);
		return;
	}

	json assignments;
	try {
		file >> assignments;
	}
	catch (const json::parse_error& e) {
		wxMessageBox(wxString::Format("JSON parse error: %s", e.what()), "Error", wxOK | wxICON_ERROR);
		return;
	}

	file.close();

	if (assignments.empty()) {
		wxMessageBox("No assignments found in the JSON file", "Info", wxOK | wxICON_INFORMATION);
		return;
	}

	for (const auto assignment : assignments) {
		if (assignment.contains("assignment_name")) {
			std::string assignment_name = assignment["assignment_name"].get<std::string>();
			assignment_vector.push_back(assignment_name);

			//wxString wx_assignment_name = wxString::FromUTF8(assignment_name.c_str());
			//assignment_list->AppendString(wx_assignment_name);
		}
		else {
			wxMessageBox("Missing 'assignment_name' in JSON", "Error", wxOK | wxICON_ERROR);
		}
	}
}


void InstructorFrame::update_students_for_assignment(const std::string& assignment_name) {
	student_list->Clear();
	get_students();

	//for (const auto student : student_vector) {
		//wxString wx_student = wxString::FromUTF8(student.c_str());  // Safe conversion
		//student_list->AppendString(wx_student); // Eventually, display grades here
	//}
}

void InstructorFrame::on_select_assignment(wxCommandEvent& event) {
	wxString selected_assignment = assignment_list->GetStringSelection();
	if (!selected_assignment.IsEmpty()) {
		update_students_for_assignment(selected_assignment.ToStdString());
	}
}
