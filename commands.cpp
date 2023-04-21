
#include	"./Headers/Utils.hpp"
// #include	<sstream>
// #include	<iostream>
// # include 	<sys/socket.h>

void	Server::Command_PASS(std::string str, int user_id)
{
	std::string tmp;
	std::cout << "passwort = "<< str <<" size " <<str.size()<< std::endl;
	std::cout << "pollfd.fd = "<< _fds[user_id].fd << std::endl;
	tmp = str.substr(0, str.find_first_of("\r\n"));
	// std::cout << "$" << tmp << "$" << std::endl;
	// std::cout << "$" << str << "$" << std::endl;
	if ( _users[user_id]._valid_password == true )
	{
		send(_fds[user_id].fd, ":Server ERROR Password already is correct\r\n", 44, 0);
		return ;
	}
	else if (!str.empty() && get_password().compare(tmp) == 0)
	{
		send(_fds[user_id].fd, ":Server INFO Password is correct\r\n", 35, 0);
		_users[user_id]._valid_password = true;
	}
	else
	{
		send(_fds[user_id].fd, ":Server ERROR Password is false\r\n", 34, 0);
		_users[user_id]._valid_password = false;
	}
}

void Server::Command_JOIN(TranslateBNF msg ,int user_id)
{
	(void)user_id;
	std::string channelName = msg.getter_params()[0].trailing_or_middle;
	std::string nickname = msg.getter_params()[1].trailing_or_middle;
	std::string password = msg.getter_params()[2].trailing_or_middle;
	int channelIndex = getChannel(this->_channels, channelName);
	int userIndex = getUser(this->_users, channelName);
	bool userExists = this->_channels[channelIndex].userExists(nickname);
	Channel	*currentChannel = &this->_channels[channelIndex];
	User	&currentUser = this->_users[userIndex];

	// create channel if it doesn't exist yet
	if (channelIndex == -1)
	{
		// return error - ERR_NOSUCHCHANNEL
		Channel newChannel(channelName);
		this->_channels.push_back(newChannel);
		channelIndex = getChannel(this->_channels, channelName);
	}
	if (channelIndex != -1 && userExists)
	{
		if (currentChannel->_settings->inviteOnly) // check if user is invited
		{
			if (this->_channels[channelIndex].isInvited(nickname))
			{
				this->_channels[channelIndex].join(currentUser);
				// return RPL_TOPIC
			}
			else
			{
				// return error - ERR_INVITEONLYCHAN
			}
		}
		else if (currentChannel->_settings->password.length() > 0) // check if password for channel is correct
		{
			if (this->_channels[channelIndex]._settings->password == password)
			{
				this->_channels[channelIndex].join(currentUser);
				// return RPL_TOPIC
			}
			else
			{
				// return error - ERR_BADCHANNELKEY
			}
		}
		else if (!currentChannel->_settings->inviteOnly && currentChannel->_settings->password.length() != 0) // Everyone can join
		{
			this->_channels[channelIndex].join(currentUser);
			// return RPL_TOPIC
		}
	}
}

void Server::Command_KICK(TranslateBNF msg,int user_id)
{
	(void)user_id;
	std::string channelName = msg.getter_params()[0].trailing_or_middle;
	std::string nickname = msg.getter_params()[1].trailing_or_middle;
	std::string nicknameToBeKicked = msg.getter_params()[1].trailing_or_middle;
	std::string comment = msg.getter_params()[2].trailing_or_middle;
	int channelIndex = getChannel(this->_channels, channelName);
	bool userExists = this->_channels[channelIndex].userExists(nickname);
	bool userToBeKickedExists = this->_channels[channelIndex].userExists(nicknameToBeKicked);
	Channel	*currentChannel = &this->_channels[channelIndex];

	if (channelIndex != -1 && userExists && userToBeKickedExists)
	{
		if (currentChannel->isAdmin(nickname)) // check if user that wants to kick has privileges
		{
			currentChannel->kick(nicknameToBeKicked);
			if (comment.length() > 0)
			{
				// send message to client with [comment] variable
			}
			else
			{
				// send message to client without a comment
			}
		}
		else
		{
			// return error - ERR_CHANOPRIVSNEEDED
		}
	}
	else if (channelIndex != -1)
	{
		// return error - ERR_NOSUCHCHANNEL
	}
	else if (!userToBeKickedExists)
	{
		// return error - ERR_NOTONCHANNEL
	}
}

void	Server::Command_TOPIC(TranslateBNF msg,int user_id)
{
	(void)user_id;
	std::string channelName = msg.getter_params()[0].trailing_or_middle;
	std::string nickname = msg.getter_params()[1].trailing_or_middle;
	std::string topic = msg.getter_params()[2].trailing_or_middle;
	int channelIndex = getChannel(this->_channels, channelName);
	bool userExists = this->_channels[channelIndex].userExists(nickname);
	Channel	*currentChannel = &this->_channels[channelIndex];

	if (channelIndex != -1 && userExists)
	{
		if (topic.length() == 0)
		{
			if (currentChannel->getTopic().length() > 0)
			{
				// return - RPL_TOPIC
			}
			else
			{
				// return - RPL_NOTOPIC
			}
		}
		else
		{
			if (currentChannel->isAdmin(nickname))
			{
				currentChannel->setTopic(nickname, topic);
				// return - RPL_TOPIC
			}
			else
			{
				// return - ERR_CHANOPRIVSNEEDED
			}
		}
	}
	else if (!userExists)
	{
		// return error - ERR_NOTONCHANNEL
	}
}

void Server::Command_NICK(TranslateBNF msg, int user_id)
{
	(void)user_id;
	if (_users[user_id]._valid_nickname == false && msg.getter_params()[0].trailing_or_middle.empty()){
		_users[user_id].setNickname(msg.getter_params()[0].trailing_or_middle);
		_users[user_id]._valid_nickname = true;
	}
	else {
	}
	//call Nick_func
	std::cout << "nickname: "<< _users[user_id].getNickname() << std::endl;
}

void	Server::Command_PART(TranslateBNF msg, int user_id)
{
	(void)user_id;
	std::string channelName = msg.getter_params()[0].trailing_or_middle;
	std::string nickname = msg.getter_params()[1].trailing_or_middle;
	int channelIndex = getChannel(this->_channels, channelName);
	bool userExists = this->_channels[channelIndex].userExists(nickname);
	Channel	*currentChannel = &this->_channels[channelIndex];
	if (channelIndex != -1 && userExists)
		currentChannel->part(nickname);
	else if (channelName.length() == 0)
	{
		// return error - ERR_NEEDMOREPARAMS
	}
	else if (channelIndex == -1)
	{
		// return error - ERR_NOSUCHCHANNEL
	}
	else if (!userExists)
	{
		// return error - ERR_NOTONCHANNEL
	}
}

void	Server::Command_P_MSG(TranslateBNF msg, int user_id)
{
	(void)user_id;
	(void)msg;
}
void	Server::Command_MODE(TranslateBNF msg, int user_id)
{
	(void)user_id;
	(void)msg;
}

