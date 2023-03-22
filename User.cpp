#include "./Headers/User.hpp"

User::User( std::string username, std::string nickname ) : username(username), nickname(nickname)
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

const std::string User::getNickname( void )
{
	return (this->nickname);
}