#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <iostream>

using json = nlohmann::json;

class User {
public:
	User();

	User(const std::string& username,
		const std::string& password,
		const std::string& role,
		const std::vector<std::string>& completed);
	~User() = default;

	User(User&& other) noexcept;

	User& operator=(User&& other) noexcept;

	User(const User& other);

	User& operator=(const User& other);

	bool operator==(const User& other);
	bool operator!=(const User& other);

	std::string get_username() const;
	std::string get_role() const;

	bool check_password(std::string entered_password);
	friend void to_json(json& j, const User& model);
	friend void from_json(const json& j, User& model);

private:
	std::string role;
	std::string username;
	std::string password;
	std::vector<std::string> completed;
};