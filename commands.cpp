
#include	"./Headers/Server.hpp"
#include	"./Headers/Connection.hpp"
#include	<sstream>
#include	<iostream>
# include 	<sys/socket.h>

void	Server::Command_PASS(std::string str, int iter)
{
	std::cout << "passwort = "<< str << std::endl;
	std::cout << "pollfd.fd = "<< _con[iter]._pollfd->fd << std::endl;
	if ( _con[iter]._valid_password == true )
	{
		send(_con[iter]._pollfd->fd, ":Server ERROR Password already is correct\r\n", 44, 0);
		return ;
	}
	else
		_con[iter].set_password(str);
	if (get_password().compare(_con[iter].get_password()))
	{
		send(_con[iter]._pollfd->fd, ":Server INFO Password is correct\r\n", 35, 0);
		_con[iter]._valid_password = true;
	}
	else
	{
		send(_con[iter]._pollfd->fd, ":Server ERROR Password is false\r\n", 34, 0);
		_con[iter]._valid_password = false;
	}
}