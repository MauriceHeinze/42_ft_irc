
#include	"./Headers/Server.hpp"
#include	"./Headers/Connection.hpp"
#include	<sstream>
#include	<iostream>
# include 	<sys/socket.h>

void	Server::Command_PASS(Connection& con, std::string str)
{
	std::cout << "passwort = "<< str << std::endl;
	std::cout << "pollfd.fd = "<< con._pollfd->fd << std::endl;
	if ( con._valid_password == true )
	{
		send(con._pollfd->fd, ":Server ERROR Password already is correct\r\n", 44, 0);
		return ;
	}
	else
		con.set_password(str);
	if (get_password().compare(con.get_password()))
	{
		send(con._pollfd->fd, ":Server INFO Password is correct\r\n", 35, 0);
		con._valid_password = true;
	}
	else
	{
		send(con._pollfd->fd, ":Server ERROR Password is false\r\n", 34, 0);
		con._valid_password = false;
	}
}