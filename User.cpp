#include "./Headers/User.hpp"

User::User( std::string username ) : username(username)
{

}

// User::User(const User &a)
// {

// }

User::~User()
{

}

// User& User::operator= (const User& a)
// {

// 	return (*(this));
// }

const std::string& User::getUsername( void )
{
	return (this->username);
}

void User::setNickname( std::string name)
{
	this->nickname = name;
}

const std::string User::getNickname( void )
{
	return (this->nickname);
}

const std::string User::getFullname( void )
{
	return (this->fullName);
}