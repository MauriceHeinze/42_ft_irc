#include "./Headers/Utils.hpp"

// bool	isChannel(std::vector<Channel> &channels, std::string channelName)
// {
// 	for (size_t i = 0; i < channels.size(); i++)
// 	{
// 		if (channels[i].getName() == channelName)
// 			return (true);
// 	}
// 	return (false);
// }

// bool	isUser(std::vector<User> users, std::string Name)
// {
// 	for (size_t i = 0; i < users.size(); i++)
// 	{
// 		if (users[i].getNickname() == Name)
// 			return (true);
// 	}
// 	return (false);
// }

int	Server::find_Channel(std::string channelName)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].getName() == channelName)
			return (i);
	}
	return (-1);
}

int	Server::find_User(std::vector<User> users, std::string nickname)
{
	for (size_t i = 0; i < users.size(); i++)
	{
		if (users[i].getNickname() == nickname)
			return (i);
	}
	return (-1);
}

int	Server::find_Username(std::vector<User> users, std::string username)
{
	for (size_t i = 0; i < users.size(); i++)
	{
		if (users[i].getUsername() == username)
			return (i);
	}
	return (-1);
}

int	argsNeeded(std::string flags)
{
	int argCounter = 0;

	if (flags[0] == '+')
	{
		if (flags.find('k') != std::string::npos)
			argCounter++;
		if (flags.find('o') != std::string::npos)
			argCounter++;
		if (flags.find('l') != std::string::npos)
			argCounter++;
	}
	return (argCounter);
}

void	Server::update_channel_nickname(std::string new_nickname,int user_id)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		_channels[i].update_nickname(new_nickname, user_id);
	}
	
}
