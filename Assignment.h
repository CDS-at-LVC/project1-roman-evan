/*#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include <wx/wx.h>
#include <wx/filepicker.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <ctime>

class Assignment {
public:
	Assignment(std::string assignment_name, std::vector<std::string> input, std::vector<std::string> output, time_t due_date);

	std::string get_assignment_name();
	std::vector<std::string> get_input_files();
	std::vector<std::string> get_output_files();
	time_t get_due_date();

	bool is_past_due();




private:
	std::vector<std::string> input_files;
	std::vector<std::string> output_files;
	std::string assignment_name;
	time_t due_date;
};

#endif*/