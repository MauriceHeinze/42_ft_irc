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
		_channels[i].leave_user(&_users[user_id]);//, "User got diconnected"
		if (_channels[i].empty() == true)
			_channels.erase(_channels.begin() + i);

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

#define RPL_WELCOME(client, networkname, nick) "001 " + client + " :Welcome to the " + networkname + " Network, " + nick + "\r\n"

#define RPL_YOURHOST(client, servername, version) "002 " + client + " :Your host is " + servername + ", runnig version " + version + "\r\n"

#define RPL_CREATED(client, datetime) "003 " + client + " :this server was created " + datetime + "\r\n"

#define RPL_MYINFO(client, servername, version, availble_user_modes, available_channel_modes) "004 " + client + " " + servername + " " + version + " " + availble_user_modes + " " + available_channel_modes + "\r\n"

#define RPL_ISUPPORT(client, token_list) "005 " + client + " " + token_list + " :are supportedby this server" + "\r\n"

void	Server::send_WELCOME(int user_id)
{
	out("send Welcome")
	// std::string client("Client_t");
	// std::string networkname("Networkname_t");
	// std::string nick = _users[user_id].getNickname();
	// std::string servername("Servername_t");
	// std::string datetime("datetime_t");
	// std::string version("version");
	// send_msg(RPL_WELCOME("client", ""),user_id);
	// send_msg("",user_id);
	// send_msg("",user_id);
	// send_msg("",user_id);
	(void)user_id;
}


void	Server::parsing(std::string buffer, int user_id)
{
 	TranslateBNF msg(buffer);

	if (_users[user_id]._send_welcome == false && this->_users[user_id]._valid_password == true && this->_users[user_id]._valid_nickname == true && this->_users[user_id]._valid_username == true)
	{
		send_WELCOME(user_id);
		_users[user_id]._send_welcome = true;
	}
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
	else if (this->_users[user_id]._valid_password == false || this->_users[user_id]._valid_nickname == false || this->_users[user_id]._valid_username == false)
	{
		this->send_msg("Pass username or nickname incorrect\r\n",user_id);
		//maybe more info what is missing 
		return ;
	}

	// protection for everthing that need Password_valid
	// protection for everthing that need valid_nick
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
void	Server::remove_user_from_all_channels( int user_id)
{
	for (size_t i = 0; i < this->_channels.size() ; i++)
	{
		int user_in_channel = _channels[i].find_user_in_channel(_users[user_id].getNickname());
		if (user_in_channel != -1)
		{
			this->_channels[i].leave_user(&_users[user_id]);
		}
	}
	
}