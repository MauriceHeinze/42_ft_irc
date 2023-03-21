
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