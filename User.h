#pragma once
#include <string>


class User
{
public:
	User(const std::string& username): username{username}
	{

	}

	const std::string username;

	friend void from_json() {

	}

private:
	std::string password;
};

