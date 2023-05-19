#include "./Headers/Utils.hpp"

bool	isChannel(std::vector<Channel> &channels, std::string channelName)
{
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i].getName() == channelName)
			return (true);
	}
	return (false);
}

bool	isUser(std::vector<User> users, std::string channelName)
{
	for (size_t i = 0; i < users.size(); i++)
	{
		if (users[i].getNickname() == channelName)
			return (true);
	}
	return (false);
}

int	Server::find_Channel(std::string channelName)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].getName() == channelName)
			return (i);
	}
	out("No channel :" + channelName)
	return (-1);
}

int	Server::find_User(std::vector<User> users, std::string nickname)
{
	for (size_t i = 0; i < users.size(); i++)
	{
		if (users[i].getNickname() == nickname)
			return (i);
	}
	out("No user :" + nickname)
	return (-1);
}

int	argsNeeded(std::string flags)
{
	int argCounter = 0;

	if (flags.find('k') != std::string::npos)
		argCounter++;
	if (flags.find('o') != std::string::npos)
		argCounter++;
	if (flags.find('l') != std::string::npos)
		argCounter++;
	return (argCounter);
}