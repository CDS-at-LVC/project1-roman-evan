#include "../Header Files/Submission.h"

Submission::Submission() {}

Submission::Submission(const std::string& assignment_id,
    const std::string& username,
    bool accepted,
    bool passed,
    int tests_passed,
    int total_tests,
    const std::string& submission_time)
    : assignment_id(assignment_id),
    username(username),
    accepted(accepted),
    passed(passed),
    tests_passed(tests_passed),
    total_tests(total_tests),
    submission_time(submission_time) {}

Submission::Submission(Submission&& other) noexcept
    : assignment_id(std::move(other.assignment_id)),
    username(std::move(other.username)),
    accepted(std::move(other.accepted)),
    passed(std::move(other.passed)),
    tests_passed(std::move(other.tests_passed)),
    total_tests(std::move(other.total_tests)),
    submission_time(std::move(other.submission_time)) {}

Submission& Submission::operator=(Submission&& other) noexcept {
    if (this != &other) {
        assignment_id = std::move(other.assignment_id);
        username = std::move(other.username);
        accepted = std::move(other.accepted);
        passed = std::move(other.passed);
        tests_passed = std::move(other.tests_passed);
        total_tests = std::move(other.total_tests);
        submission_time = std::move(other.submission_time);
    }
    return *this;
}

Submission::Submission(const Submission& other)
    : assignment_id(other.assignment_id),
    username(other.username),
    accepted(other.accepted),
    passed(other.passed),
    tests_passed(other.tests_passed),
    total_tests(other.total_tests),
    submission_time(other.submission_time) {}

Submission& Submission::operator=(const Submission& other) {
    if (this != &other) {
        assignment_id = other.assignment_id;
        username = other.username;
        accepted = other.accepted;
        passed = other.passed;
        tests_passed = other.tests_passed;
        total_tests = other.total_tests;
        submission_time = other.submission_time;
    }
    return *this;
}

bool Submission::operator==(const Submission& other) const {
    return assignment_id == other.assignment_id;
}

bool Submission::operator!=(const Submission& other) const {
    return !(*this == other);
}

std::string Submission::get_assignment_id() const {
    return assignment_id;
}

std::string Submission::get_username() const {
    return username;
}

bool Submission::get_accepted() const {
    return accepted;
}

bool Submission::get_passed() const {
    return passed;
}

int Submission::get_tests_passed() const {
    return tests_passed;
}

int Submission::get_total_tests() const {
    return total_tests;
}

std::string Submission::get_submission_time() const {
    return submission_time;
}

void to_json(json& j, const Submission& model) {
    j = json{
        {"assignment_id", model.assignment_id},
        {"username", model.username},
        {"accepted", model.accepted},
        {"passed", model.passed},
        {"tests_passed", model.tests_passed},
        {"tests_total", model.total_tests},
        {"submission_time", model.submission_time}
    };
}

void from_json(const json& j, Submission& model) {
    j.at("assignment_id").get_to(model.assignment_id);
    j.at("username").get_to(model.username);
    j.at("accepted").get_to(model.accepted);
    j.at("passed").get_to(model.passed);
    j.at("tests_passed").get_to(model.tests_passed);
    j.at("tests_total").get_to(model.total_tests);
    j.at("submission_time").get_to(model.submission_time);
}