#include <ctime>
#include "Submission.h"
#include <vector>
#include <string>
#define _CRT_SECURE_NO_WARNINGS

Submission::Submission(std::string student_name, std::string file_path, time_t timestamp)
	: student(student_name), submission_file(file_path), submission_time(timestamp), grade(0), passed(false) {};

void Submission::setGrade(double new_grade) {
	grade = new_grade;
}

double Submission::getGrade() const {
	return grade;
}

bool Submission::isSubmissionPassed() const {
	return passed;
}
	
void Submission::setPassed(bool pass_status) {
	passed = pass_status;
}
	

std::string Submission::getSubmissionFile() const {
	return submission_file;
};
time_t Submission::getSubmissionTime() const {
	return submission_time;
};



