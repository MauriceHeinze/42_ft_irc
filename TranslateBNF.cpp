#include	"./Headers/Utils.hpp"

#define	letter		"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define	number		"0123456789"
#define	special		"-[]\\`ˆ{}"
#define	nonnonwhite	"\n\r "
//#define	non_empty	"\32\10\13\0" // first one cant be ':'
//#define empty 		"\10\13\0"


namespace tools{
const std::string _letter(letter);
const std::string _number(number);
const std::string _special(special);
const std::string _nonnonwhite(nonnonwhite);
};

namespace error{
	TranslateBNF msg();
}

void	TranslateBNF::deconstruct_prefix(std::string& msg,int& i)
{
	i++;
	prefix.nick = true;
	if (tools::_letter.find(msg[i]))
	{
		while(tools::_letter.find(msg[i]) != std::string::npos|| tools::_number.find(msg[i]) != std::string::npos|| tools::_special.find(msg[i])!= std::string::npos)
		{
			prefix.nick_or_servername.push_back(msg[i]);
			i++;
		}
	}
	else
	{
		//std::cout << "Error" << std::endl;
	}
	if (msg[i] == '!')
	{
		i++;
		while (tools::_nonnonwhite.find(msg[i]) == std::string::npos && msg[i] != 0 && msg[i] != '@')
		{
			prefix.user.push_back(msg[i]);
			i++;
		}
	}
	// this is not just a placeholder for now
	if (msg[i] == '@')
	{
		i++;
		while (tools::_nonnonwhite.find(msg[i]) == std::string::npos && msg[i] != 0)
		{
			prefix.host.push_back(msg[i]);
			i++;
		}
	}
	i++;
}

void	TranslateBNF::deconstruct_command(std::string& msg,int& i)
{
	if ( msg[i] != '\0' && tools::_letter.find(msg[i]) != std::string::npos)
	{
		while (msg[i] != '\0' && tools::_letter.find(msg[i]) != std::string::npos)
		{
			command.command.push_back(msg[i]);
			i++;
		}
	}
	else
	{
		std::cout << "No Valid Command" << std::endl;
	}
	int nums = 0;
	while (msg[i] != 0 && tools::_number.find(msg[i]) != std::string::npos)
	{
		command.num.push_back(msg[i]);
		i++;
		nums++;
	}
	if (nums > 3)
	{
		std::cout << "Error nums" << std::endl;
	}
}

void	TranslateBNF::deconstruct_params(std::string& msg,int& i)
{
	s_param param;
	if (msg[i] != '\0' && msg[i] == ':')
	{
		i++;
		while(msg[i] != '\0' && tools::_nonnonwhite.find(msg[i]) == std::string::npos)
		{
			param.trailing_or_middle.push_back(msg[i]);
			i++;
		}
		param.trailing = false;
	}
	else
	{
		while(msg[i] != '\0' && msg[i] != ' ' && tools::_nonnonwhite.find(msg[i]) == std::string::npos)
		{
			param.trailing_or_middle.push_back(msg[i]);
			i++;
		}
	}
	params.push_back(param);
}

void	TranslateBNF::deconstruct_msg (std::string msg)
{
	int	i = 0;
	std::cout << msg << std::endl;
		if (msg[0] == ':')
	{
		deconstruct_prefix(msg , i);
	}
	deconstruct_command(msg , i);
	while (msg[i] == ' ')
		i++;
	while(msg[i] != '\r' && msg[i+1] != '\n')
	{
		while (msg[i] == ' ')
			i++;
		if (msg[i] == '\0')
		{
			std::cout << "Error false end chars" << std::endl;
			break;
		}
		else
			deconstruct_params(msg , i);
	}
	std::cout << "prefix = " << prefix.nick_or_servername << std::endl;
	std::cout << "user = " << prefix.user << std::endl;
	std::cout << "host = " << prefix.host << std::endl;
	std::cout << "command = " << command.command << std::endl;
	std::cout << "nums = " << command.num << std::endl;
	for(size_t j = 0; j < params.size();j++)
	{
		std::cout << "param " << j << " = "<< params[j].trailing_or_middle << std::endl;
	}
	std::cout <<std::endl;
}


void	TranslateBNF::set_prefix_servername(std::string servername)
{
	this->prefix.nick_or_servername = servername;
	this->prefix.nick = false;
}

void	TranslateBNF::set_prefix_nick(std::string nick)
{
	this->prefix.nick_or_servername = nick;
	this->prefix.nick = true;
}


void	TranslateBNF::set_prefix_host(std::string host)
{
	this->prefix.host = host;
}


void	TranslateBNF::set_prefix_user(std::string user)
{
	this->prefix.user = user;
}

std::string	TranslateBNF::get_prefix_user( void )
{
	return (this->prefix.user);
}

std::string	TranslateBNF::get_prefix_host( void )
{
	return (this->prefix.host);
}

void	TranslateBNF::setter_command( std::string command )
{
	this->command.command = command;
}

std::string	TranslateBNF::getter_command( void )
{
	return(this->command.command);
}

void	TranslateBNF::setter_num( std::string num )
{
	this->command.num = num;
}

std::string	TranslateBNF::getter_num( void )
{
	return(this->command.num);
}



void	TranslateBNF::setter_params( std::vector<s_param> params)
{
	this->params = params;
}


// getter_params()[0] parameter (e.g. channel name)
// getter_params()[1] nickname
std::vector<s_param>	TranslateBNF::getter_params( void )
{
	return(this->params);
}


std::string	TranslateBNF::get_prefix_servername( void )
{
	if (this->prefix.nick == false)
		return (this->prefix.nick_or_servername);
	else
		return (std::string());
}

TranslateBNF::TranslateBNF(std::string msg)
{
	deconstruct_msg(msg);
}

TranslateBNF::TranslateBNF()
{

}

TranslateBNF::TranslateBNF(const TranslateBNF &a)
{
	this->command = a.command;
	this->prefix = a.prefix;
	this->params = a.params;
}

TranslateBNF::~TranslateBNF()
{

}

TranslateBNF& TranslateBNF::operator= (const TranslateBNF& a)
{
	this->command = a.command;
	this->prefix = a.prefix;
	this->params = a.params;
	return (*(this));
}

std::string	TranslateBNF::get_full_msg( void )
{
	std::string msg;
	if (this->prefix.nick_or_servername.empty() == false)
	{
		msg.append(prefix.nick_or_servername);
		if (this->prefix.host.empty() == false)
		{
			msg.push_back('!');
			msg.append(prefix.host);
		}
		if (this->prefix.user.empty() == false)
		{
			msg.push_back('@');
			msg.append(prefix.user);
		}
		msg.push_back(' ');
	}
	msg.append(this->command.command);
	msg.append(this->command.num);
	for (size_t i = 0; i < this->params.size(); i++)
	{
		msg.push_back(' ');
		if (this->params[i].trailing == true)
		{
			msg.push_back(':');
		}
		msg.append(params[i].trailing_or_middle);
	}
	msg.append("\10\13");
	return(msg);
}

void	TranslateBNF::add_param(std::string msg, bool trailing)
{
	struct s_param param = {trailing, msg};
	this->params.push_back(param);
}
