#include "./Headers/User.hpp"

User::User( std::string username ) : username(username)
{
	_valid_password = false;
}

User::User()
{
	username = "no_name";
	_valid_password = false;
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

const std::string& User::getFullname( void )
{
	return (this->fullName);
}