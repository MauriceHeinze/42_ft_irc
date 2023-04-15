
#include	"./Headers/Server.hpp"
#include	<sstream>
#include	<iostream>
# include 	<sys/socket.h>

void	Server::Command_PASS(std::string str, int iter)
{
	std::string tmp;
	std::cout << "passwort = "<< str <<" size " <<str.size()<< std::endl;
	std::cout << "pollfd.fd = "<< _fds[iter].fd << std::endl;
	tmp = str.substr(0, str.find_first_of("\r\n"));
	// std::cout << "$" << tmp << "$" << std::endl;
	// std::cout << "$" << str << "$" << std::endl;
	if ( _users[iter]._valid_password == true )
	{
		send(_fds[iter].fd, ":Server ERROR Password already is correct\r\n", 44, 0);
		return ;
	}
	else if (!str.empty() && get_password().compare(tmp) == 0)
	{
		send(_fds[iter].fd, ":Server INFO Password is correct\r\n", 35, 0);
		_users[iter]._valid_password = true;
	}
	else
	{
		send(_fds[iter].fd, ":Server ERROR Password is false\r\n", 34, 0);
		_users[iter]._valid_password = false;
	}
}
