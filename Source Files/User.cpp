#include "../Header Files/User.h"


User::User() {}

User::User(const std::string& username,
	const std::string& password,
	const std::string& role,
	const std::vector<std::string>& completed)
	: username(username), password(password), role(role), completed(completed) {}


User::User(User&& other) noexcept
	: role(std::move(other.role)),
	username(std::move(other.username)),
	password(std::move(other.password)),
	completed(std::move(other.completed)) {}


User& User::operator=(User&& other) noexcept {
	if (this != &other) {
		role = std::move(other.role);
		username = std::move(other.username);
		password = std::move(other.password);
		completed = std::move(other.completed);
	}
	return *this;
}

User::User(const User& other)
	: username(other.username),
	password(other.password),
	role(other.role),
	completed(other.completed) {}

User& User::operator=(const User& other) {
	if (this != &other) {
		username = other.username;
		password = other.password;
		role = other.role;
		completed = other.completed;
	}
	return *this;
}

bool User::operator==(const User& other)
{
	return username == other.username && role == other.role;
}

bool User::operator!=(const User& other)
{
	return !(operator==(other));
}

std::string User::get_username() const {
	return username;
}

std::string User::get_role() const {
	return role;
}

bool User::check_password(std::string entered_password) {
	return entered_password == password;
}

void to_json(json& j, const User& model)
{
	j = json{
	{"username", model.username},
	{"role", model.role},
	{"password", model.password},
	{"completed", model.completed}
	};
}

void from_json(const json& j, User& model) {
	j.at("username").get_to(model.username);
	j.at("role").get_to(model.role);
	j.at("password").get_to(model.password);
	j.at("completed").get_to(model.completed);
}