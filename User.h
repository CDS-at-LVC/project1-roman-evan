#pragma once
#include <string>

class User {
public:

	User(std::string username, std::string password, std::string role);
	std::string get_username() const;
	std::string get_role() const;

	bool check_password(std::string entered_password);

	bool operator==(const User& user) {
		return role == user.role || username == user.username || password == user.password;
	}

private:
	std::string role;
	std::string username;
	std::string password;
};