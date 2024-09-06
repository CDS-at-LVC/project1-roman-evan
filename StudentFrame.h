#ifndef STUDENTFRAME_H
#define STUDENTFRAME_H

#include <wx/wx.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <ctime>

class StudentFrame : public wxFrame {
public:
    StudentFrame(const wxString& title);

    void get_grade_report();
    void list_pending_assignments();

    // Struct for assignments
    struct Assignment {
        std::string assignment_name;
        time_t assignment_due_date;
    };

    struct Submission {
        Assignment assigned;
        std::string result_of_compilation;
        double grade;
        bool compiled;
        bool passed_due_date;
    };

private:
    void submit_assignment(Assignment assignment);
    void onSubmitAssignment(wxCommandEvent& event);

    // Vectors for completed and uncompleted assignments to be displayed
    std::vector<Submission> submissions;
    std::vector<Assignment> assignments;

    wxListBox* assignment_list;
    wxListBox* submission_list;

    // Helper functions
    bool check_due_date(const Assignment& assignment);
    double compile_and_grade(const std::string& file_path);

    // Load assignments from a JSON or file
    std::vector<Assignment> load_assignments();

    wxDECLARE_EVENT_TABLE();
};

#endif