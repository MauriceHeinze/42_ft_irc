#include "./Headers/Utils.hpp"

User::User( std::string username, int fd) : username(username) , _fd(fd)
{
	_valid_password = false;
	_valid_nickname = false;
	_valid_username = false;
	_send_welcome = false;
}

User::User( int fd ) :_fd(fd)
{
	_valid_password = false;
	_valid_nickname = false;
	_valid_username = false;
	_send_welcome = false;
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

User& User::operator= (const User& other)
{
	this->username = other.username;
	this->fullName = other.fullName;
	this->nickname = other.nickname;
	this->_valid_nickname = other._valid_nickname;
	this->_valid_password = other._valid_password;
	this->msg = other.msg;
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

// void	User::send_msg_to_Channels(std::string msg)
// {
// 	for (size_t i = 0; i < connected_channel.size(); i++)
// 	{
// 		connected_channel[i]->send_to_all(msg);
// 	}
// }