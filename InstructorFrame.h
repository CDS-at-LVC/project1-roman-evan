#ifndef INSTRUCTORFRAME_H
#define INSTRUCTORFRAME_H

#include <wx/wx.h>
#include <wx/filepicker.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <ctime>

class InstructorFrame : public wxFrame {
public:

	InstructorFrame(const wxString& title);

	void create_assignment(std::string assignment_name, std::vector<std::string> input_files, std::vector<std::string> output_files, time_t due_date);
	void delete_assignment(std::string assignment_name);

	void on_create_assignment(wxCommandEvent& event);
	void on_delete_assignment(wxCommandEvent& event);
	void on_select_assignment(wxCommandEvent& event);

	std::vector<std::string> get_students();

private:
	std::vector<std::string> student_vector;
	std::vector<std::string> assignment_vector;

	wxListBox* assignment_list;
	wxListBox* student_list;

	void update_assignments();
	void update_students_for_assignment(const std::string& assignment_name);

};



#endif
