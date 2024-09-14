#include "../Header Files/Assignment.h"


Assignment::Assignment() {}

Assignment::Assignment(const std::string& assignment_id,
    const std::string& due_date,
    std::vector<std::string> input_files,
    std::vector<std::string> output_files,
    bool deleted)
    : assignment_id(assignment_id),
    due_date(due_date), input_files(input_files), output_files(output_files), deleted(deleted) {}

Assignment::Assignment(Assignment&& other) noexcept
    : assignment_id(std::move(other.assignment_id)),
    due_date(std::move(other.due_date)),
    input_files(std::move(other.input_files)),
    output_files(std::move(other.output_files)),
    deleted(std::move(other.deleted)){}

Assignment& Assignment::operator=(Assignment&& other) noexcept {
    if (this != &other) {
        assignment_id = std::move(other.assignment_id);
        due_date = std::move(other.due_date);
        input_files = std::move(other.input_files);
        output_files = std::move(other.output_files);
        deleted = std::move(other.deleted);
    }
    return *this;
}

Assignment::Assignment(const Assignment& other)
    : assignment_id(other.assignment_id),
    due_date(other.due_date),
    input_files(other.input_files),
    output_files(other.output_files),
    deleted(other.deleted){}

Assignment& Assignment::operator=(const Assignment& other) {
    if (this != &other) {
        assignment_id = other.assignment_id;
        due_date = other.due_date;
        input_files = other.input_files;
        output_files = other.output_files;
        deleted = other.deleted;
    }
    return *this;
}

bool Assignment::operator==(const Assignment& other) const {
    return assignment_id == other.assignment_id;
}

bool Assignment::operator!=(const Assignment& other) const {
    return !(*this == other);
}

std::string Assignment::get_assignment_id() const {
    return assignment_id;
}

std::string Assignment::get_due_date() const {
    return due_date;
}

std::vector<std::string> Assignment::get_inputs() const {
    return input_files;
}

std::vector<std::string> Assignment::get_outputs() const {
    return output_files;
}

bool Assignment::is_deleted() const {
    return deleted;
}

void Assignment::set_deleted(bool del) {
    deleted = del;
}

void to_json(json& j, const Assignment& model) {
    j = json{
        {"id", model.assignment_id},
        {"due_date", model.due_date},
        {"deleted", model.deleted},
        {"tests", json::object()}
    };

    // Make sure the sizes of input_files and output_files match before adding them to JSON
    if (model.input_files.size() == model.output_files.size()) {
        for (size_t i = 0; i < model.input_files.size(); ++i) {
            j["tests"][model.input_files[i]] = model.output_files[i];
        }
    }
    else {
        throw std::runtime_error("Mismatch between input_files and output_files sizes");
    }
}

void from_json(const json& j, Assignment& model) {
    // Ensure the required fields are present before accessing
    if (j.contains("id") && j.contains("due_date") && j.contains("tests")) {
        j.at("id").get_to(model.assignment_id);
        j.at("due_date").get_to(model.due_date);
        j.at("deleted").get_to(model.deleted);

        model.input_files.clear();
        model.output_files.clear();

        // Iterate through the "tests" object and fill input/output vectors
        const auto& tests = j.at("tests");
        for (auto it = tests.begin(); it != tests.end(); ++it) {
            model.input_files.push_back(it.key());
            model.output_files.push_back(it.value().get<std::string>());
        }
    }
    else {
        throw std::runtime_error("Missing required fields in JSON");
    }
}