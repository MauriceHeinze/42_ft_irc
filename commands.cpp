
#include	"./Headers/Server.hpp"
#include	"./Headers/Connection.hpp"
#include	<sstream>
#include	<iostream>
# include 	<sys/socket.h>

void	Server::Command_PASS(std::string str, int iter)
{
	std::cout << "passwort = "<< str <<" size " <<str.size()<< std::endl;
	std::cout << "pollfd.fd = "<< _fds[iter].fd << std::endl;
	if ( _users[iter]._valid_password == true )
	{
		send(_fds[iter].fd, ":Server ERROR Password already is correct\r\n", 44, 0);
		return ;
	}
	if (get_password().compare(str) == 0)
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
