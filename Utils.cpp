#include "./Headers/Utils.hpp"

bool	isChannel(std::vector<Channel> channels, std::string channelName)
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

int	getChannel(std::vector<Channel> channels, std::string channelName)
{
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i].getName() == channelName)
			return (i);
	}
	return (-1);
}

int	getUser(std::vector<User> users, std::string nickname)
{
	for (size_t i = 0; i < users.size(); i++)
	{
		if (users[i].getNickname() == nickname)
			return (i);
	}
	return (-1);
}