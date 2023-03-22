
#ifndef COMMAND_H
# define COMMAND_H

#include	"Server.hpp"
#include	"Connection.hpp"
#include	<string>

void	Command_PASS( Server& , Connection& , std::string );

#endif