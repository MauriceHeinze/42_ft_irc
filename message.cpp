
#include	"./Headers/message.hpp"
#include	<iostreams>

#define	letter "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define	number "0123456789"
#define	special "-[]\\`ˆ{}"
#define nonnonwhite "\32\10\13\0" 

namespace tools{
const std::string _letter(letter);
const std::string _number(number);
const std::string _special(special);
const std::string _nonnonwhite(nonnonwhite);
};

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
		while (tools::_nonnonwhite.find(msg[i]) == 0)
		{
			prefix.nick_or_servername.push_back(msg[i]);
			i++;
		}
	}
}

void	message::deconstruct_command(std::string& msg,int& i)
{
	if (tools::_letter.find(msg[i]))
	{
		do{
			command.command.push_back(msg[i]);
			i++;
		}
		while (tools::_letter.find(msg[i]))
	}
	int nums = 0;
	while (tools::_number.find(msg[i]))
	{
		command.num.push_back(msg[i]);
		i++;
		num++;
	}
	if (num > 3 || num < 2)
	{
		std::cout << "Error" << std::endl;
	}
}
void	message::deconstruct_params(std::string& msg,int& i)
{
	
}
void	message::deconstruct_msg (std::string msg)
{
	int	i = 0;

	if (msg.front() == ':')
	{
		deconstruct_prefix(msg , i);
	}
	deconstruct_command(msg , i);
}

message::message(std::string msg)
{
	deconstruct_msg(msg);
}

message::message(const message &a)
{
	
}

message::~message()
{
	
}

message& message::operator= (const message& a)
{
	
	return (*(this));
}
