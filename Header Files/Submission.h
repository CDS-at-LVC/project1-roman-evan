#pragma once

#include <iomanip>
#include <sstream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

class Submission {
public:
    Submission();

    Submission(const std::string& assignment_id,
        const std::string& assignment_name,
        const std::string& username,
        bool accepted,
        bool passed,
        int tests_passed,
        int total_tests,
        const std::string& submission_time);

    Submission(Submission&& other) noexcept;

    Submission& operator=(Submission&& other) noexcept;

    Submission(const Submission& other);

    Submission& operator=(const Submission& other);

    bool operator==(const Submission& other) const;
    bool operator!=(const Submission& other) const;

    std::string get_assignment_id() const;
    std::string get_assignment_name() const;
    std::string get_username() const;
    bool get_accepted() const;
    bool get_passed() const;
    int get_tests_passed() const;
    int get_total_tests() const;
    std::string get_submission_time() const;

    friend void to_json(json& j, const Submission& model);
    friend void from_json(const json& j, Submission& model);

private:
    std::string assignment_id;
    std::string assignment_name;
    std::string username;
    bool accepted;
    bool passed;
    int tests_passed;
    int total_tests;
    std::string submission_time;
};
