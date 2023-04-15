#include "./Headers/User.hpp"

User::User( std::string username ) : username(username)
{
	_valid_password = false;
	_valid_nickname = false;
}

User::User()
{
	username = "no_name";
	_valid_password = false;
	_valid_nickname = false;
}
// User::User(const User &a)
// {

// }

User::~User()
{

}

User& User::operator= (const User& a)
{
	(void)a;
	return (*(this));
}

void	User::setUsername( const std::string &name)
{
	this->username = name;
}

const std::string& User::getUsername( void )
{
	return (this->username);
}

void	User::setNickname( const std::string &name)
{
	this->nickname = name;
}

const std::string& User::getNickname( void )
{
	return (this->nickname);
}

void	User::setFullname( const std::string &name)
{
	this->fullName = name;
}

const std::string& User::getFullname( void )
{
	return (this->fullName);
}