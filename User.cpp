#include <string>
#include "User.h"

User::User(std::string username, std::string password, std::string role) : username(username), password(password), role(role) {};

std::string User::get_username() const {
	return username;
}

std::string User::get_role() const {
	return role;
}

bool User::check_password(std::string entered_password) {
	return entered_password == password;
}
