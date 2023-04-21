#include "./Headers/Utils.hpp"

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

void	User::insert_in_user_buffer(std::string read_buffer)
{
	this->msg.append(read_buffer);
	std::cout << "current Buffer" << std::endl <<this->msg << std::endl;
}

// Get the next complete(\r\n) command inside of the msg_buffer or return a empty if no one is found
std::string	User::get_next_command( void )
{
	std::string command;

	size_t crnl = (this->msg.find("\r\n"));
	// std::cout << crnl << " len" << std::endl;
	if (crnl == std::string::npos)
		return("");
	else
		crnl += 2;
	//store a  copy to return later
	command = this->msg;
	command.resize(crnl);
	//set the buffer to  the next point 
	this->msg.erase(0,crnl);
	return (command);
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