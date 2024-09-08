#ifndef SUBMISSION_H
#define SUBMISSION_H

#include <string>
#include <ctime>

class Submission {
public:
    Submission(std::string student_name, std::string file_path, time_t timestamp);


    void setGrade(double new_grade);
    double getGrade() const;

    bool isSubmissionPassed() const;
    void setPassed(bool pass_status);

    std::string getSubmissionFile() const;
    time_t getSubmissionTime() const;

private:
    std::string student;
    std::string submission_file;
    time_t submission_time;
    double grade;
    bool passed;
};

#endif