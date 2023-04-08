#include	"./Headers/BNF_Translate.hpp"
#include	<iostream>

#define	letter		"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define	number		"0123456789"
#define	special		"-[]\\`ˆ{}"
#define	nonnonwhite	"\32\10\13\0" 
//#define	non_empty	"\32\10\13\0" // first one cant be ':'
//#define empty 		"\10\13\0"


namespace tools{
const std::string _letter(letter);
const std::string _number(number);
const std::string _special(special);
const std::string _nonnonwhite(nonnonwhite);
};

void	BNF_Translate::deconstruct_prefix(std::string& msg,int& i)
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
		while (tools::_nonnonwhite.find(msg[i]) == std::string::npos && msg[i] != 0 && msg[i] != ' ')
		{
			prefix.host.push_back(msg[i]);
			i++;
		}
	}
	i++;
}

void	BNF_Translate::deconstruct_command(std::string& msg,int& i)
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

void	BNF_Translate::deconstruct_params(std::string& msg,int& i)
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

void	BNF_Translate::deconstruct_msg (std::string msg)
{
	int	i = 0;
	std::cout << msg << std::endl;
	std::cout << msg.front()<<std::endl;
	if (msg.front() == ':')
	{
		deconstruct_prefix(msg , i);
	}
	deconstruct_command(msg , i);
	while (msg[i] == ' ')
		i++;
	while(msg[i] != '\10' && msg[i+1] != '\13')
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


void	BNF_Translate::set_prefix_servername(std::string servername)
{
	this->prefix.nick_or_servername = servername;
	this->prefix.nick = false;
}

void	BNF_Translate::set_prefix_nick(std::string nick)
{
	this->prefix.nick_or_servername = nick;
	this->prefix.nick = true;
}


void	BNF_Translate::set_prefix_host(std::string host)
{
	this->prefix.host = host;
}


void	BNF_Translate::set_prefix_user(std::string user)
{
	this->prefix.user = user;
}

std::string	BNF_Translate::get_prefix_user( void )
{
	return (this->prefix.user);
}

std::string	BNF_Translate::get_prefix_host( void )
{
	return (this->prefix.host);
}

void	BNF_Translate::setter_command( std::string command )
{
	this->command.command = command;
}

std::string	BNF_Translate::getter_command( void )
{
	return(this->command.command);
}

void	BNF_Translate::setter_num( std::string num )
{
	this->command.num = num;
}

std::string	BNF_Translate::getter_num( void )
{
	return(this->command.num);
}



void	BNF_Translate::setter_params( std::vector<s_param> params)
{
	this->params = params;
}

std::vector<s_param>	BNF_Translate::getter_params( void )
{
	return(this->params);
}


std::string	BNF_Translate::get_prefix_servername( void )
{
	if (this->prefix.nick == false)
		return (this->prefix.nick_or_servername);
	else
		return (std::string());
}

BNF_Translate::BNF_Translate(std::string msg)
{
	deconstruct_msg(msg);
}

BNF_Translate::BNF_Translate()
{
	
}

BNF_Translate::BNF_Translate(const BNF_Translate &a)
{
	this->command = a.command;
	this->prefix = a.prefix;
	this->params = a.params;
}

BNF_Translate::~BNF_Translate()
{
	
}

BNF_Translate& BNF_Translate::operator= (const BNF_Translate& a)
{
	this->command = a.command;
	this->prefix = a.prefix;
	this->params = a.params;
	return (*(this));
}

std::string	BNF_Translate::get_full_string( void )
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
