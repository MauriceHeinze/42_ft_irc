
#include	"./Headers/Server.hpp"
#include	"./Headers/Connection.hpp"

void	Command_PASS(Server& serv, Connection& con)
{
	std::cout << "imhere" << std::endl;
	if (serv.get_password().compare(con.get_password()))
		con._valid_password = true;
	else
		con._valid_password = false;
}

/*
	// client commands
	- set nickname
	- set username
	- join channel
	- have private messages (PRIVMSG) and notices (NOTICE) fully working
	- have different priviliges for regular users and operators
	- set operator


	// channel operation commands
	- set topic (MODE)
	- create channel
	-
*/