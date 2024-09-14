#include "Assignment.h"


Assignment::Assignment() {}

Assignment::Assignment(const std::string& assignment_id,
	const std::string& due_date,
	std::vector<std::string> input_files,
	std::vector<std::string> output_files,
	bool active)
	: assignment_id(assignment_id),	due_date(due_date), active(active)
{
	for (int i = 0; i < input_files.size(); ++i)
	{
		dataFiles[input_files[i]] = output_files[i];
	}
}

Assignment::Assignment(Assignment&& other) noexcept
	: assignment_id(std::move(other.assignment_id)),
	due_date(std::move(other.due_date)),
	active(std::move(other.active)),
	dataFiles(std::move(other.dataFiles)) {}

Assignment& Assignment::operator=(Assignment&& other) noexcept {
	if (this != &other) {
		assignment_id = std::move(other.assignment_id);
		due_date = std::move(other.due_date);
		active = std::move(other.active);
		dataFiles = std::move(other.dataFiles);
	}
	return *this;
}

Assignment::Assignment(const Assignment& other)
	: assignment_id(other.assignment_id),
	due_date(other.due_date),
	active(other.active),
	dataFiles(other.dataFiles) {}

Assignment& Assignment::operator=(const Assignment& other) {
	if (this != &other) {
		assignment_id = other.assignment_id;
		due_date = other.due_date;
		active = other.active;
		dataFiles = other.dataFiles;
	}
	return *this;
}

bool Assignment::operator==(const Assignment& other) const {
	return assignment_id == other.assignment_id;
}

bool Assignment::operator!=(const Assignment& other) const {
	return !(*this == other);
}

bool Assignment::isActive() const {
	return active;
}

std::string Assignment::get_assignment_id() const {
	return assignment_id;
}

std::string Assignment::get_due_date() const {
	return due_date;
}

const std::unordered_map<std::string, std::string>& Assignment::getDataFiles() const {
	return dataFiles;
}

void Assignment::deleteAssignment()
{
	active = false;
}

void to_json(json& j, const Assignment& model) {
	j = json{
		{"id", model.assignment_id},
		{"due_date", model.due_date},
		{"tests", json::object()},
		{"active", model.active}
	};
	for (const auto& entry : model.dataFiles) {
		j["tests"][entry.first] = entry.second;
	}

}

void from_json(const json& j, Assignment& model) {
	// Ensure the required fields are present before accessing
	if (j.contains("id") && j.contains("due_date") && j.contains("tests")) {
		j.at("id").get_to(model.assignment_id);
		j.at("due_date").get_to(model.due_date);
		j.at("active").get_to(model.active);

		model.dataFiles.clear();

		// Iterate through the "tests" object and fill input/output vectors
		const auto& tests = j.at("tests");
		for (auto it = tests.begin(); it != tests.end(); ++it) {
			model.dataFiles[it.key()] = it.value().get<std::string>();
		}
	}
	else {
		throw std::runtime_error("Missing required fields in JSON");
	}
}