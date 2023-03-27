
#include	"./Headers/Message.hpp"
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
#define deggree_rad(x)  x+1
message::message()
{
	
}

void	message::deconstruct_prefix(std::string& msg,int& i)
{
	i++;
	prefix.nick = true;
	if (tools::_letter.find(msg[i]))
	{
		do{
			prefix.nick_or_servername.push_back(msg[i]);
			i++;
		}
		while(tools::_letter.find(msg[i]) || tools::_number.find(msg[i]) || tools::_special.find(msg[i]));
	}
	else
	{
		std::cout << "Error" << std::endl;
	}
	if (msg[i] == '!')
	{
		i++;
		while (tools::_nonnonwhite.find(msg[i]) == 0)
		{
			prefix.nick_or_servername.push_back(msg[i]);
			i++;
		}
	}
	// this is not just a placeholder for now
	if (msg[i] == '@')
	{
		i++;
		while (tools::_nonnonwhite.find(msg[i]) == 0 )
		{
			prefix.nick_or_servername.push_back(msg[i]);
			i++;
		}
	}
}

void	message::deconstruct_command(std::string& msg,int& i)
{
	if ( !msg[i] && tools::_letter.find(msg[i]))
	{
		do{
			command.command.push_back(msg[i]);
			i++;
		}
		while (!msg[i] && tools::_letter.find(msg[i]));
	}
	int nums = 0;
	while (!msg[i] && tools::_number.find(msg[i]))
	{
		command.num.push_back(msg[i]);
		i++;
		nums++;
	}
	// if (nums > 3 || nums < 2)
	// {
	// 	std::cout << "Error" << std::endl;
	// }
}
void	message::deconstruct_params(std::string& msg,int& i)
{
	s_param param;
	if (!msg[i] && msg[i] == ':')
	{
		i++;
		while(tools::_nonnonwhite.find(msg[i]) == 0)
		{
			param.trailing_or_middle.push_back(msg[i]);
		}
		param.trailing = false;
	}
	else
	{
		while(!msg[i] && msg[i] != ' ' && tools::_nonnonwhite.find(msg[i]) == 0)
		{
			param.trailing_or_middle.push_back(msg[i]);
		}
	}
	if (!msg[i] && msg[i] == ' ')
	{
		i++;
		deconstruct_params(msg , i);
	}
	params.push_back(param);
}
void	message::deconstruct_msg (std::string msg)
{
	int	i = 0;
	std::cout << msg << std::endl;
	if (!msg[i] && msg.front() == ':')
	{
		deconstruct_prefix(msg , i);
	}
	std::cout << "after prefix" << std::endl;
	deconstruct_command(msg , i);
	std::cout << "after command" << std::endl;
	if (msg[i] == ' ')
	{
		i++;
		deconstruct_params(msg , i);
	}
	std::cout << "after params" << std::endl;
	if (msg[i] == '\10' && msg[i+1] == '\13')
		return;
	else
		std::cout << "Error" << std::endl;

	std::cout << "prefix = " << prefix.nick_or_servername << std::endl;
	std::cout << "user = " << prefix.user << std::endl;
	std::cout << "host = " << prefix.host << std::endl;
	std::cout << "command = " << command.command << std::endl;
	std::cout << "nums = " << command.num << std::endl;
	std::cout << "param 1 = " << params[0].trailing_or_middle << std::endl;
	std::cout << "param 2 = " << params[1].trailing_or_middle << std::endl;
	std::cout << "param 3 = " << params[2].trailing_or_middle << std::endl;
}

message::message(std::string msg)
{
	deconstruct_msg(msg);
}

message::message(const message &a)
{
	(void)a;
}

message::~message()
{
	
}

message& message::operator= (const message& a)
{
	
	(void)a;
	return (*(this));
}
