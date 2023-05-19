// #include 	"./Headers/Server.hpp"
// #include	<sstream>
// #include	"./Headers/Commands.hpp"
#include	"./Headers/Utils.hpp"
#include	<stdlib.h>

#define SEND_FLAGS 0
#define LAST_USER 0


void	Server::send_msg(std::string msg,int user_id)
{
	out("\e[31m" + msg + "\e[0m")
	send(this->_fds[user_id].fd,msg.c_str(),msg.size(), SEND_FLAGS);
}

Server::Server(){
	return ;
}

Server::Server(char **argv)
{
	_port = atoi(argv[1]);
	_password = argv[2];
}

Server::~Server(){
	return ;
}

void Server::setSocket(){
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(_socket, F_SETFL, O_NONBLOCK);
	if(_socket == -1)
		throw(SocketCreationFail());
	int optval = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1){
		std::cerr << "setsockopt failed" << std::endl;
		return ;
	}
	sockaddr_in servAddress;
	servAddress.sin_family = AF_INET;
	servAddress.sin_addr.s_addr = INADDR_ANY;
	servAddress.sin_port = htons(_port);
	if (bind(_socket, (sockaddr *)&servAddress, sizeof(servAddress)) == -1)
		throw(SocketBindingFail());
	if (listen(_socket, 20) == -1)
		throw(SocketListenFail());
	pollfd fd = {_socket, POLLIN, 0};
	_fds.push_back(fd);
	User bot(_socket);
	_users.push_back(bot);
}

void	Server::delete_user(int user_id)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		_channels[i].leave_user(&_users[user_id], "User got diconnected");
	}
	close(this->_fds[user_id].fd);
	this->_fds.erase(this->_fds.begin() + user_id);
	this->_users.erase(this->_users.begin() + user_id);
}

void Server::startServer(){
	try {
		setSocket();
	}
	catch(std::exception& e) {
		std::cerr << e.what() << std::endl;
		return ;
	}

	while (1) {
		int pollVal = poll(_fds.data(), _fds.size(), 5000);
		std::cout << "current connections " << _fds.size() - 1 << std::endl;
		if (pollVal == -1)
			throw(PollFail());
		for (size_t i = 0; i < _fds.size(); i++) 
		{
			if ( i && _fds[i].revents & (POLLHUP | POLLERR | POLLNVAL))
			{
				out("delete user");
				delete_user(i);
			}
			else if (_fds[i].revents & POLLIN)
			{
				if (i == 0)
				{
					out("acceptConnection")
					acceptConnection();
					out("END")
				}
				else
				{
					out("recvMsg")
					recvMsg(i);
				}
			}
		}
	}
	close( _socket);
}

void Server::acceptConnection()
{
	sockaddr_in clientAddress;
	socklen_t clientAddressSize = sizeof(clientAddress);
	int clientSocket = accept(_socket, (sockaddr *)&clientAddress, &clientAddressSize);
	fcntl(clientSocket, F_SETFL, O_NONBLOCK);
	if (clientSocket ==  -1)
		throw(ClientAcceptFail());
	else
		std::cout << "Client connected!" << "using the fd =" << clientSocket <<std::endl;
	pollfd new_fd = {clientSocket, POLLIN, 0};
	_fds.push_back(new_fd);
	User new_user(clientSocket);
	_users.push_back(new_user);
	//! Need better opening
	// send_msg(":Server opening :Hallo, was geht\r\n",clientSocket);
}



void	Server::parsing(std::string buffer, int user_id)
{
 	TranslateBNF msg(buffer);

	if (msg.getter_command() == "CAP")
	{
		Command_CAP(msg, user_id);
	}
	else  if (msg.getter_command() == "PING")
	{
		Command_PING(msg, user_id);
	}
	else if (msg.getter_command() == "USER")
	{
		Command_USER(msg, user_id);
	}
	else if (msg.getter_command() == "PASS")
	{
		Command_PASS(msg, user_id);
	}
	else if (msg.getter_command() == "NICK")
	{
		Command_NICK(msg, user_id);
	}
	else if (this->_users[user_id]._valid_password == false)//check here if passwort is vaild
	{
		// send(this->_fds[user_id].fd,)
		return ;
	}
	// protection for everthing that need Password_valid
	// protection for everthing that need valid_nick
	else if (this->_users[user_id]._valid_nickname == false)//check here if passwort is vaild
	{
		
		// send(this->_fds[user_id].fd,)
		out("Nickname")
		out(this->_users[user_id].getNickname())
		out("\e[0;31merror invalid Nick\e[0m")
		send_msg(":Server Invalid Nickname", user_id);
		return ;
	}
	else if (msg.getter_command() == "JOIN")
	{
		Command_JOIN(msg ,user_id);
	}
	else if (msg.getter_command() == "WHO")
	{
		Command_WHO(msg, user_id);
	}
	else if (msg.getter_command() == "KICK")
	{
		Command_KICK(msg,user_id);
	} 
	else if (msg.getter_command() == "TOPIC")
	{
		Command_TOPIC(msg, user_id);
	}
	else if (msg.getter_command() == "PART")
	{
		Command_PART(msg, user_id);
	}
	else if (msg.getter_command() == "MODE")
	{
		Command_MODE(msg, user_id);
	}
	else if (msg.getter_command() == "PRIVMSG")
	{
		Command_P_MSG(msg, user_id);
	}
	else if (msg.getter_command() == "INVITE")
	{
		Command_INVITE(msg, user_id);
	}
	else
	{
		out("no Command found")
		//!needs Return a Error rpl
	}
}


void Server::recvMsg(size_t user_id)
{
	char buffer[1024];
	int valread;
	memset(buffer, 0, 1024); //set the buffer to 0
	valread = recv(_fds[user_id].fd, buffer, 1024, 0);
	if (valread == 0)// check for error 
	{
		std::cout << "Connection Closed " << user_id << std::endl;
		return ;
	}
	this->_users[user_id].insert_in_user_buffer(buffer);
	while (1)
	{
		std::string command= this->_users[user_id].get_next_command();
		if (command[0] == 0)
			break;
		parsing(command, user_id);
	}
	_fds[user_id].revents = 0;
	_fds[user_id].events = POLLIN;
}

std::string	Server::get_password( void )
{
	return(_password);
}


//in case for diconnect
/**
 * @brief iterate throu all channels and remove the user if he was inside one, also sends all other users a msg that someone left
 * @param msg the msg that the other users resieve
 * @param user_id the fd/Socket/user that left the channel
 * 
 */
void	Server::remove_user_from_all_channels(std::string msg, int user_id)
{
	for (size_t i = 0; i < this->_channels.size() ; i++)
	{
		int user_in_channel = _channels[i].find_user_in_channel(_users[user_id].getNickname());
		if (user_in_channel != -1)
		{
			this->_channels[i].leave_user(&_users[user_id], msg);
		}
	}
	
}