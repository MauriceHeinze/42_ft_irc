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

bool	isUser(std::vector<User> users, std::string Name)
{
	for (size_t i = 0; i < users.size(); i++)
	{
		if (users[i].getNickname() == Name)
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