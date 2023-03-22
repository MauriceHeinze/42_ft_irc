#include "./Headers/User.hpp"

User::User( std::string username, std::string nickname ) : username(username)
{

}

User::User(const User &a)
{

}

User::~User()
{

}

User& User::operator= (const User& a)
{

	return (*(this));
}

const std::string User::getUsername( void )
{
	return (this->username);
}

void User::setNickname( std::string name)
{
	this->nickname = name;
}

std::string User::getNickname( void )
{
	return (this->nickname);
}