#pragma once
#include <iomanip>
#include <sstream>
#include <unordered_map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Assignment {
public:
    Assignment();

    Assignment(const std::string& assignment_id,
        const std::string& due_date,
        std::vector<std::string> input_files,
        std::vector<std::string> output_files,
        bool active);


    Assignment(Assignment&& other) noexcept;


    Assignment& operator=(Assignment&& other) noexcept;

    Assignment(const Assignment& other);

    Assignment& operator=(const Assignment& other);

    bool operator==(const Assignment& other) const;
    bool operator!=(const Assignment& other) const;
    bool isActive() const;


    std::string get_assignment_id() const;

    std::string get_due_date() const;

    const std::unordered_map<std::string, std::string>& getDataFiles() const;

    void deleteAssignment();

    void friend to_json(json& j, const Assignment& model);
    void friend from_json(const json& j, Assignment& model);
private:
    std::string assignment_id;
    std::string due_date;
    std::vector<std::string> input_files;
    std::vector<std::string> output_files;
    std::unordered_map<std::string, std::string> dataFiles;
    bool active;

};
