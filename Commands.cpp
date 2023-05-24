#include	"./Headers/Utils.hpp"

void	Server::Command_PASS(TranslateBNF msg, int user_id)
{
	std::string str;
	if ( _users[user_id]._valid_password == true )
	{
		send_msg(ERR_ALREADYREGISTRED, user_id);
		return ;
	}
	if(msg.getter_params().size() > 0)
		str = msg.getter_params()[0].trailing_or_middle;
	else
	{
		send_msg(ERR_NEEDMOREPARAMS(this->_users[user_id].getNickname(), msg.getter_command()), user_id);
		return ;
	}
	if (!str.empty() && get_password().compare(str) == 0)
	{
		send_msg("462 :password correct\r\n", user_id);
		_users[user_id]._valid_password = true;
	}
	else
	{
		std::cout << get_password().size() << " | " << str.size() << std::endl;
		send_msg(ERR_PASSWDMISMATCH, user_id);
		_users[user_id]._valid_password = false;
	}
}

void Server::Command_USER(TranslateBNF msg ,int user_id)
{
	if (msg.getter_params().size() > 0 && msg.getter_params()[0].trailing_or_middle.empty() == false)
	{
		int user_index = find_Username(_users,msg.getter_params()[0].trailing_or_middle);
		if (user_index  == -1)
		{
			_users[user_id].setUsername(msg.getter_params()[0].trailing_or_middle);
		}
		else
		{
			send_msg(ERR_ALREADYREGISTRED, user_id);
		}
	}
	else
		this->send_msg(ERR_NONICKNAMEGIVEN(),user_id);
	std::cout << "U: "<< _users[user_id].getNickname() << std::endl;
}

//:irc.example.com 353 your_nick #channel_name :@user1 +user2 user3

//new channel
 void Server::create_new_channel(std::string new_channel ,int user_id, std::string channel_password)
{
	//!check for valid channel name
	Channel Channel(new_channel, channel_password, _users[user_id]);
	//!give first user admin rights
	//send list of user and topics if succesfull
	_channels.push_back(Channel);
	_channels.back().send_to_all(RPL_JOIN(_users[user_id].getNickname(), new_channel));
}

//(SERVER_NAME " 353 " + request.get_user()->get_nickname() + " = " + channel.getName() + " :" + info, request.get_user()->get_fd()
//old channel
 void Server::use_old_channel(int channel_id, int user_id, std::string channel_password)
{
	//joining inside our channel class
	out("use old")
	int rpl_msg = _channels[channel_id].add_new_user(_users[user_id], channel_password);
	// for reply
	if (rpl_msg == rpl_ERR_BADCHANNELKEY)
		this->send_msg(ERR_BADCHANNELKEY(_users[user_id].getNickname(), _channels[channel_id].getName()), user_id);
	else if (rpl_msg == rpl_ERR_INVITEONLYCHAN)
		this->send_msg(ERR_INVITEONLYCHAN(_users[user_id].getNickname(), _channels[channel_id].getName()), user_id);
	else if (rpl_msg == rpl_default)
	{
		this->_channels[channel_id].send_to_all(RPL_JOIN(_users[user_id].getNickname(), _channels[channel_id].getName()));
		this->send_msg(RPL_NAMREPLY(_users[user_id].getNickname(),_channels[channel_id].getName(),_channels[channel_id].get_user_list()),user_id);
		this->send_msg(RPL_ENDOFNAMES(_users[user_id].getNickname(),_channels[channel_id].getName()),user_id);
	}
	else if (rpl_msg == rpl_no_rpl)
		return;
	else
	{
		out("error missing !!!!(use_old_channel)")
		return;
	}
}
void Server::Command_JOIN(TranslateBNF msg ,int user_id)
{
	std::vector<s_param> params = msg.getter_params();
	std::string channel_password;
	std::string channel;
	//input protection in case to many or to little params
	if (params.size() > 2 || params.size() == 0)
	{
		this->send_msg("Error in JOIN , needs to replaced\r\n",user_id);
		return;
	}
	if (params.size() > 0)
	{
		channel = params[0].trailing_or_middle; // testing
		if (channel[0] != '#')// not sure about thus one
			channel = "#" + channel;
	}
	if (params.size()> 1)
		channel_password = params[1].trailing_or_middle;
	else
		channel_password = "";
	//looks if the channel already is created
	int channel_id = this->find_Channel(channel);
	out(channel_id)
	if (channel_id != -1)
	{
		if (_channels[channel_id]._settings.userLimit == _channels[channel_id].getPerms().size())
		use_old_channel(channel_id, user_id, channel_password);
	}
	else
	{
		create_new_channel(channel ,user_id, channel_password);
	}
	return;
}

void Server::Command_WHO(TranslateBNF msg, int user_id)
{
	out("send who")
	if (msg.getter_params().size() > 0)
	{
		std::string channelName = msg.getter_params()[0].trailing_or_middle;
		out(channelName)
		out(find_Channel(channelName))
		if (find_Channel(channelName) != -1)
		{
			send_msg(RPL_WHOREPLY(_users[user_id].getNickname(), channelName, "kvirc", "127.0.0.1", "blublub", _users[user_id].getNickname(),"0" , "lgollong"), user_id);
			send_msg(RPL_ENDOFWHO(channelName), user_id);
		}
		else
			send_msg(ERR_NOSUCHSERVER(_users[user_id].getNickname(), channelName), user_id);
	}

}

void Server::Command_KICK(TranslateBNF msg,int user_id)
{
	std::string nickname = _users[user_id].getNickname();
	if (msg.getter_params().size() > 1){
		std::string channelName = msg.getter_params()[0].trailing_or_middle;
		if (channelName.find("#") == std::string::npos)
			channelName = "#" + channelName;
		int channelIndex = find_Channel(channelName);
		std::string kickNick = msg.getter_params()[1].trailing_or_middle;
		if (channelIndex == -1)
			send_msg(ERR_NOSUCHCHANNEL(nickname,channelName),user_id);
		else if (_channels[channelIndex].find_user_in_channel(kickNick) == -1)
			send_msg(ERR_NOSUCHNICK(nickname),user_id);
		else if (_channels[channelIndex].isAdmin(_users[user_id].getNickname())){
			if (msg.getter_params().size() == 2){
				_channels[channelIndex].send_to_all(":" + nickname + " KICK " + channelName + " " + kickNick + "\r\n");
				_channels[channelIndex].leave_user(&_users[find_User(_users,kickNick)]);
			}
			else if (msg.getter_params().size() == 3){
				std::string reason = msg.getter_params()[2].trailing_or_middle;
				_channels[channelIndex].send_to_all(":" + nickname + " KICK " + channelName + " " + kickNick + " :" + reason + "\r\n");
				_channels[channelIndex].leave_user(&_users[find_User(_users,kickNick)]);
			}
		}
		else
			send_msg(ERR_CHANOPRIVSNEEDED(nickname, channelName), user_id);
	}
	else
		send_msg(ERR_NEEDMOREPARAMS(nickname, "KICK"), user_id);
}

void	Server::Command_TOPIC(TranslateBNF msg,int user_id)
{
	std::string nickname = _users[user_id].getNickname();
	if (msg.getter_params().size() > 0)
	{
		std::string channelName = msg.getter_params()[0].trailing_or_middle;
		if (channelName.find("#") == std::string::npos)
			channelName = "#" + channelName;
		int channelIndex = this->find_Channel(channelName);
		Channel	*currentChannel = &this->_channels[channelIndex];
		int userExists = currentChannel->find_user_in_channel(nickname);
		std::string topic = msg.getter_params()[1].trailing_or_middle;
		if (userExists == -1)
			this->send_msg(ERR_NOTONCHANNEL(nickname, channelName), user_id);
		else if (channelIndex != -1 && userExists > -1)
		{
			if (topic.length() == 0)
			{
				if (currentChannel->getTopic().length() > 0)
					this->send_msg(RPL_TOPIC(nickname, channelName, currentChannel->getTopic()), user_id);
				else
					this->send_msg(RPL_NOTOPIC(nickname, channelName), user_id);
			}
			else
			{
				if (currentChannel->isAdmin(nickname) || currentChannel->_settings.topicOperatorOnly == false)
				{
					currentChannel->setTopic(nickname, topic);
					this->send_msg(RPL_TOPIC(nickname, channelName, currentChannel->getTopic()), user_id);
				}
				else
					this->send_msg(ERR_CHANOPRIVSNEEDED(nickname, channelName), user_id);
			}
		}
		else if (channelIndex == -1)
			this->send_msg(ERR_NOSUCHCHANNEL(nickname, channelName), user_id);
	}
	else
		this->send_msg(ERR_NEEDMOREPARAMS(nickname, "TOPIC"), user_id);
}

void	Server::Command_NICK(TranslateBNF msg, int user_id)
{
	if (msg.getter_params().size() > 0 && msg.getter_params()[0].trailing_or_middle.empty() == false)
	{
		int index = find_User(_users, msg.getter_params()[0].trailing_or_middle);
		if (index == -1)
		{
			send_msg(":" + _users[user_id].getNickname() + " NICK :" + msg.getter_params()[0].trailing_or_middle + "\r\n", user_id);
			_users[user_id].setNickname(msg.getter_params()[0].trailing_or_middle);
			_users[user_id]._valid_nickname = true;
		}
		else
				this->send_msg(ERR_NICKNAMEINUSE(_users[user_id].getNickname()),user_id);
	}
	else
		this->send_msg(ERR_NONICKNAMEGIVEN(),user_id);
	//call Nick_func
}

void	Server::Command_PART(TranslateBNF msg, int user_id)
{
	std::string nickname = _users[user_id].getNickname();
	if (msg.getter_params().size() > 0){
		std::string channelName = msg.getter_params()[0].trailing_or_middle;
		if (channelName.find("#") == std::string::npos)
			channelName = "#" + channelName;
		int channelIndex = find_Channel(channelName);
		if (channelIndex == -1)
			send_msg(ERR_NOSUCHCHANNEL(nickname,channelName),user_id);
		else if (_channels[channelIndex].find_user_in_channel(nickname) == -1)
			send_msg(ERR_NOTONCHANNEL(nickname, channelName), user_id);
		else if (msg.getter_params().size() == 1){
			_channels[channelIndex].send_to_all(":" + nickname + " PART " + channelName + "\r\n");
			_channels[channelIndex].leave_user(&_users[user_id]);
		}
		else if (msg.getter_params().size() == 2){
			std::string reason = msg.getter_params()[1].trailing_or_middle;
			_channels[channelIndex].send_to_all(":" + nickname + " PART " + channelName + " :" + reason + "\r\n");
			_channels[channelIndex].leave_user(&_users[user_id]);
		}
	}
	else
		send_msg(ERR_NEEDMOREPARAMS(nickname, "PART"), user_id);
}

#define PRIVTMSG(nickname,target,message) ":" + nickname + " PRIVMSG " + target + " :" + message + "\r\n"
void	Server::Command_P_MSG(TranslateBNF msg, int user_id)
{
	if (msg.getter_params().size() > 0)
	{
		std::string	target = msg.getter_params()[0].trailing_or_middle;
		if (msg.getter_params()[0].trailing_or_middle[0] == '#'){
			int i = this->find_Channel(target);
			if (i == -1)
				this->send_msg(ERR_NOSUCHCHANNEL(_users[user_id].getNickname(),target),user_id);
			else
			{
				this->_channels[i].send_to_not_all(" ",user_id);
			}
		}
		else
		{
			int target_user = find_User(_users, target);
			send_msg(PRIVTMSG(_users[user_id].getNickname(),msg.getter_params()[0].trailing_or_middle,msg.getter_params()[1].trailing_or_middle), target_user);
			send_msg(PRIVTMSG(_users[user_id].getNickname(),msg.getter_params()[0].trailing_or_middle,msg.getter_params()[1].trailing_or_middle), user_id);
		}
	}
	else
		send_msg(ERR_NEEDMOREPARAMS(this->_users[user_id].getNickname(), msg.getter_command()), user_id);
}

#define rpl_PONG(msg) "PONG" + msg + "\r\n"

void	Server::Command_PING(TranslateBNF msg, int user_id)
{
	(void) msg;
	std::string text(" params ");
	send_msg(rpl_PONG(text), user_id);
}

void	Server::Command_CAP(TranslateBNF msg, int user_id)
{
	(void)msg;
	std::cout << "Cap send" << std::endl;
	send_msg(":irc.unknown.net CAP * LS :\r\n", user_id);
}

void	Server::Command_MODE(TranslateBNF msg, int user_id)
{
	std::string nickname = this->_users[user_id].getNickname();
	std::string	channelName = "";
	if (msg.getter_params().size() > 0)
		channelName = msg.getter_params()[0].trailing_or_middle;
	int			channelIndex = this->find_Channel(channelName);
	Channel		*currentChannel = &this->_channels[channelIndex];

	std::string	argument = "";
	int			argumentsNeeded;
	int			k = 2;

	std::cout << ">>>>>>>> number of args: " << msg.getter_params().size() << std::endl;
	// check if enough params are available
	if (msg.getter_params().size() == 1) // you should return current settings at this point
	{
		std::cout << "JUST MODE" << std::endl;
		if (channelIndex != -1)
		{
			std::cout << "Channel exists" << std::endl;
			currentChannel->send_to_all(RPL_CHANNELMODEIS(nickname, channelName, currentChannel->getSettings()));
		}
		else
		{
			send_msg(ERR_NOSUCHCHANNEL(nickname, channelName), user_id);
			return ;
		}
		return ;
	}
	if (channelIndex == -1)
	{
		send_msg(ERR_NOSUCHCHANNEL(nickname, channelName), user_id);
		return ;
	}

	size_t		i = 0;
	bool		setting = false; // needed to set settings
	std::string	flags = msg.getter_params()[1].trailing_or_middle;
	argumentsNeeded = argsNeeded(flags);

	if (channelName[0] == '#')
	{
		while (i != flags.length())
		{
			if ((unsigned long)argumentsNeeded > (msg.getter_params().size() - 2))
			{
				send_msg(ERR_NEEDMOREPARAMS(nickname, "MODE"), user_id);
				return ;
			}
			if (argumentsNeeded != 0)
				argument = msg.getter_params()[k].trailing_or_middle;
			// check if upcoming flags are activating or deactivating
			if (flags[i] == '+')
				setting = true;
			else if (flags[i] == '-')
				setting = false;

			if (currentChannel->isAdmin(nickname))
			{
				if (flags[i] == 'i') // i: Set/remove Invite-only channelkl ,
					currentChannel->_settings.privateChannel = setting;
				else if (flags[i] == 't') // t: Set/remove the restrictions of the TOPIC command to channel operators
					currentChannel->_settings.topicOperatorOnly = setting;
				else if (flags[i] == 'k') // k: Set/remove the channel key (password)
				{
					std::cout << "// k: Set password to " << argument << std::endl;
					if (setting == true)
					{
						currentChannel->_settings.password = argument;
						k++;
					}
					else
						currentChannel->_settings.password = "";
				}
				else if (flags[i] == 'o') // o: Give/take channel operator privilege
				{
					std::cout << "// o: Give/take channel operator privilege to: " << argument << std::endl;
					if (this->_channels[channelIndex].userExists(argument))
						currentChannel->oper(argument);
					else
					{
						send_msg(ERR_NOTONCHANNEL(nickname, argument), user_id);
						return ;
					}
					k++;
				}
				else if (flags[i] == 'l') // l: Set/remove the user limit to channel
				{
					if (setting == false)
						currentChannel->_settings.userLimit = INT_MAX;
					else
					{
						try {
							currentChannel->_settings.userLimit = std::stoi(argument); }
						catch (const std::out_of_range& e) {
							currentChannel->_settings.userLimit = INT_MAX; }
						k++;
					}
				}
				else if (flags[i] == 'b') // just for KVIRC
					(void)flags[i];
				else if (flags[i] != '+' && flags[i] != '-')
				{
					send_msg(ERR_UNKNOWNMODE(nickname, flags[i]), user_id);
					return ;
				}
			}
			else
			{
				send_msg(ERR_CHANOPRIVSNEEDED(nickname, channelName), user_id);
				return ;
			}
			i++;
		}
	}
	else
	{
		send_msg(ERR_NOSUCHCHANNEL(nickname, channelName), user_id);
		return ;
	}
}

void Server::Command_INVITE(TranslateBNF msg, int user_id)
{
	std::string nickname = _users[user_id].getNickname();
	if (msg.getter_params().size() > 1)
	{
		std::string channelName = msg.getter_params()[1].trailing_or_middle;
		int channel = find_Channel(channelName);
		std::string invNick = msg.getter_params()[0].trailing_or_middle;
		out(invNick)
		int user = find_User(_users, invNick);
		out(user);
		if (user != -1){
			if (_channels[channel].isInvited(invNick)){
				send_msg(ERR_USERONCHANNEL(invNick, channelName), user_id);
				return ;
			}
			_channels[channel].invite(invNick);
			send_msg(RPL_INVITING(_users[user_id].getNickname(), invNick, channelName), user_id);
		}
		else
			send_msg(ERR_NOSUCHNICK(invNick), user_id);
	}
	else
		send_msg(ERR_NEEDMOREPARAMS(nickname, "INVITE"), user_id);
}

