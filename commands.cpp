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
	// (void)user_id;
	if (msg.getter_params().size() > 0)
	{
		if (!msg.getter_params()[0].trailing_or_middle.empty()){
			_users[user_id].setUsername(msg.getter_params()[0].trailing_or_middle);
		}
		else if (isUser(_users, msg.getter_params()[0].trailing_or_middle))
			send_msg(ERR_ALREADYREGISTRED, user_id);
	}
	else
		this->send_msg(ERR_NEEDMOREPARAMS((std::string)"*", (std::string)"USER"),user_id);
	//call Nick_func
	std::cout << "U: "<< _users[user_id].getNickname() << std::endl;
}

//:irc.example.com 353 your_nick #channel_name :@user1 +user2 user3

//new channel
 void Server::create_new_channel(std::string new_channel ,int user_id, std::string channel_password)
{
	//!check for valid channel name

	//create the new channel
	Channel Channel(new_channel, channel_password, _users[user_id]);
	//!give first user admin rights
	//send list of user and topics if succesfull
	_channels.push_back(Channel);
	_channels.back().send_to_all(RPL_JOIN(_users[user_id].getNickname(), new_channel));
}

//(SERVER_NAME " 353 " + request.get_user()->get_nickname() + " = " + channel.getName() + " :" + info, request.get_user()->get_fd()
#define RPL_TEST(nickname,channel,User_list) ":irc.server.com 353 "+ nickname + " = " + channel + " " + User_list + "\r\n"

//old channel
 void Server::use_old_channel(int channel_id, int user_id, std::string channel_password)
{
	//joining inside our channel class
	int rpl_msg = _channels[channel_id].add_new_user(_users[user_id], channel_password);
	// for reply
	if (rpl_msg == rpl_ERR_BADCHANNELKEY)
		this->send_msg(ERR_BADCHANNELKEY(_users[user_id].getNickname(), _channels[channel_id].getName()), user_id);
	else if (rpl_msg == rpl_ERR_INVITEONLYCHAN)
		this->send_msg(ERR_INVITEONLYCHAN(_users[user_id].getNickname(), _channels[channel_id].getName()), user_id);
	else if (rpl_msg == rpl_default)
	{
		this->_channels[channel_id].send_to_all(RPL_JOIN(_users[user_id].getNickname(), _channels[channel_id].getName()));
		//this->send_msg(RPL_NAMREPLY(_users[user_id].getNickname(),_channels[channel_id].getName(),_channels[channel_id].get_user_list()),user_id);
		this->send_msg(RPL_TEST(_users[user_id].getNickname(),_channels[channel_id].getName(),_channels[channel_id].get_user_list()),user_id);
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

//std::string topic("default");
//std::string user1("user_alpha");
//std::string user2("user_better");
//this->send_msg(":lkrabbe! JOIN :abc\r\n",user_id);
// this->send_msg(":irc.unknown.com 332 lkrabbe abc :default\r\n",user_id);
// this->send_msg(RPL_TOPIC(nickname, channel, topic), user_id);
// this->send_msg(USER_LIST(nickname, channel, user1, user2),user_id);
// this->send_msg(":irc.example.com 353 " + + "lkrabbe = abc : user1 user2 user3 @lkrabbe\r\n",user_id);
// this->send_msg((":irc.example.com 353  #abc :@lkrabbe +user2 user3\r\n"),user_id);
// this->send_msg((":server_name 366 your_nick #abc :End of /NAMES list\r\n"),user_id);
// this->send_msg(RPL_ENDOFNAMES(nickname, channel),user_id);

void Server::Command_JOIN(TranslateBNF msg ,int user_id)
{
	std::vector<s_param> params = msg.getter_params();
	std::string channel_password;
	std::string channel;
	// out("in join")
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
		out("use old channel")
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
	(void)user_id;
	(void)msg;
	// std::string channelName = msg.getter_params()[0].trailing_or_middle;
	// std::string nickname = msg.getter_params()[1].trailing_or_middle;
	// std::string nicknameToBeKicked = msg.getter_params()[1].trailing_or_middle;
	// std::string comment = msg.getter_params()[2].trailing_or_middle;
	// int channelIndex = this->find_Channel(channelName);
	// bool userExists = this->_channels[channelIndex].find_user_in_channel(&_users[user_id]);
	// bool userToBeKickedExists = this->_channels[channelIndex].userExists(nicknameToBeKicked);
	// Channel	*currentChannel = &this->_channels[channelIndex];

	// if (channelIndex != -1 && userExists && userToBeKickedExists)
	// {
	// 	if (currentChannel->isAdmin(nickname)) // check if user that wants to kick has privileges
	// 	{
	// 		currentChannel->kick(nicknameToBeKicked);
	// 		if (comment.length() > 0)
	// 		{
	// 			// send message to client with [comment] variable
	// 		}
	// 		else
	// 		{
	// 			// send message to client without a comment
	// 		}
	// 	}
	// 	else
	// 		this->send_msg(ERR_CHANOPRIVSNEEDED(nickname, currentChannel->getName()), user_id);
	// }
	// else if (channelIndex != -1)
	// {
	// 	this->send_msg(ERR_NOSUCHCHANNEL(nickname, currentChannel->getName()), user_id);
	// }
	// else if (!userToBeKickedExists)
	// 	this->send_msg(ERR_NOTONCHANNEL(nickname, currentChannel->getName()), user_id);
}

void	Server::Command_TOPIC(TranslateBNF msg,int user_id)
{
	if (msg.getter_params().size() > 0)
	{
		std::string channelName = msg.getter_params()[0].trailing_or_middle;
		if (channelName.find("#") == std::string::npos)
			channelName = "#" + channelName;
		int channelIndex = this->find_Channel(channelName);
		std::string nickname = _users[user_id].getNickname();
		bool userExists = isUser(_users, nickname);//this->_channels[channelIndex].userExists(nickname);
		std::string topic = msg.getter_params()[1].trailing_or_middle;
		Channel	*currentChannel = &this->_channels[channelIndex];
		if (channelIndex != -1 && userExists)
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
				if (currentChannel->isAdmin(nickname))
				{
					currentChannel->setTopic(nickname, topic);
					this->send_msg(RPL_TOPIC(nickname, channelName, currentChannel->getTopic()), user_id);
				}
				else
					this->send_msg(ERR_CHANOPRIVSNEEDED(nickname, channelName), user_id);
			}
		}
		else if (!userExists)
			this->send_msg(ERR_NOTONCHANNEL(nickname, channelName), user_id);
		else if (channelIndex == -1)
			this->send_msg(ERR_NOSUCHCHANNEL(nickname, channelName), user_id);
	}
}

void	Server::Command_NICK(TranslateBNF msg, int user_id)
{
	// (void)user_id;
	if (msg.getter_params().size() > 0)
	{
		if (!msg.getter_params()[0].trailing_or_middle.empty()){
			_users[user_id].setNickname(msg.getter_params()[0].trailing_or_middle);
			_users[user_id]._valid_nickname = true;
		}
		else if (isUser(_users, msg.getter_params()[0].trailing_or_middle))
			send_msg(ERR_NICKNAMEINUSE(msg.getter_params()[0].trailing_or_middle), user_id);
	}
	else
		this->send_msg(ERR_NONICKNAMEGIVEN(),user_id);
	//call Nick_func
	std::cout << "nickname: "<< _users[user_id].getNickname() << std::endl;
}

void	Server::Command_PART(TranslateBNF msg, int user_id)
{
	(void)user_id;
	(void)msg;
	// std::string channelName = msg.getter_params()[0].trailing_or_middle;
	// std::string nickname = msg.getter_params()[1].trailing_or_middle;
	// int channelIndex = this->find_Channel(channelName);
	// bool userExists = this->_channels[channelIndex].userExists(nickname);
	// Channel	*currentChannel = &this->_channels[channelIndex];
	// if (channelIndex != -1 && userExists)
	// 	currentChannel->part(nickname);
	// else if (channelName.length() == 0)
	// 	this->send_msg(ERR_NEEDMOREPARAMS(nickname, msg.getter_command()), user_id);
	// else if (channelIndex == -1)
	// 	this->send_msg(ERR_NOSUCHCHANNEL(nickname, currentChannel->getName()), user_id);
	// else if (!userExists)
	// 	this->send_msg(ERR_NOTONCHANNEL(nickname, currentChannel->getName()), user_id);
}

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
				this->_channels[i].send_to_all(" ",user_id);// need a send_to_with exception from sender id
			}
			// if (isChannel(_channels, msg.getter_params()[0].trailing_or_middle)){
				// send_msg(":" + _users[user_id].getNickname() + " PRIVMSG " + msg.getter_params()[0].trailing_or_middle + " :" + msg.getter_params()[1].trailing_or_middle + "\r\n", user_id);
			// }
			// else
			// 	return;
		}
		else
			send_msg(":" + _users[user_id].getNickname() + " PRIVMSG " + msg.getter_params()[0].trailing_or_middle + " :" + msg.getter_params()[1].trailing_or_middle + "\r\n", user_id);
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
	// examples:
	// MODE #Finnish +o Kilroy
	// MODE #Finnish +im

	// setze variables
	std::string nickname = this->_users[user_id].getNickname();
	// check if enough params are available
	// std::vector<s_param> arguments = msg.getter_params();
	// std::cout << "RESULT: " << arguments.size() << std::endl;
	if (msg.getter_params().size() < 1)
	{
		send_msg(ERR_NEEDMOREPARAMS(nickname, (std::string)"MODE"), user_id);
		return ;
	}

	std::cout << "first arg: " << msg.getter_params()[0].trailing_or_middle << std::endl;
	if (msg.getter_params().size() > 1)
		std::cout << "second arg: " << msg.getter_params()[1].trailing_or_middle << std::endl;
	if (msg.getter_params().size() > 2)
		std::cout << "third arg: " << msg.getter_params()[2].trailing_or_middle << std::endl;

	size_t		i = 0;
	bool		setting = false; // needed to set settings
	std::string	flags = msg.getter_params()[1].trailing_or_middle;
	std::string	argument = msg.getter_params()[2].trailing_or_middle;
	std::string	channelName = msg.getter_params()[0].trailing_or_middle;
	int			channelIndex = this->find_Channel(channelName);
	Channel		*currentChannel = &this->_channels[channelIndex];

	if (channelIndex == -1)
	{
		send_msg(ERR_NOSUCHCHANNEL(nickname, channelName), user_id);
		return ;
	}

	if (channelName[0] == '#')
	{
		while (i != flags.length())
		{
			// check if upcoming flags are activating or deactivating
			if (flags[i] == '+')
				setting = true;
			else if (flags[i] == '-')
				setting = false;

			if (currentChannel->isAdmin(nickname))
			{
				if (flags[i] == 'i') // i: Set/remove Invite-only channel
					currentChannel->_settings.privateChannel = setting;
				else if (flags[i] == 't') // t: Set/remove the restrictions of the TOPIC command to channel operators
					currentChannel->_settings.topicOperatorOnly = setting;
				else if (flags[i] == 'k') // k: Set/remove the channel key (password)
				{
					std::cout << "// k: Set/remove the channel key (password)" << std::endl;
					std::cout << "// Current password: " << currentChannel->_settings.password  << std::endl;
					if (setting == true)
						currentChannel->_settings.password = argument;
					else
						currentChannel->_settings.password = "";
					std::cout << "// New password: " << currentChannel->_settings.password  << std::endl;
				}
				else if (flags[i] == 'o') // o: Give/take channel operator privilege
				{
					std::cout << "// o: Give/take channel operator privilege" << std::endl;
					if (this->_channels[channelIndex].userExists(argument))
						currentChannel->oper(argument);
					else
						send_msg(ERR_USERONCHANNEL(nickname, argument), user_id);
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
					}
				}
				else if (flags[i] == 'b') // just for KVIRC
					(void)flags[i];
				else if (flags[i] != '+' && flags[i] != '-')
					send_msg(ERR_UNKNOWNMODE(nickname, flags[i]), user_id);
			}
			else
				send_msg(ERR_CHANOPRIVSNEEDED(nickname, channelName), user_id);
			i++;
		}
	}
	else
		send_msg(ERR_NOSUCHCHANNEL(nickname, channelName), user_id);
}
// Formatierung der msg ( chat gpt sagt <invted user> <channel> | rfc seite sagt <channel> <invited user>)
void Server::Command_INVITE(TranslateBNF msg, int user_id)
{
	if (msg.getter_params().size() > 1)
	{
		std::string nickname = _users[user_id].getNickname();
		std::string channelName = msg.getter_params()[1].trailing_or_middle;
		// int channel = find_Channel(channelName);
		std::string invNick = msg.getter_params()[0].trailing_or_middle;
		out(invNick)
		bool user = isUser(_users, invNick);
		out(user);
		if (user)
			send_msg(RPL_INVITING(_users[user_id].getNickname(), invNick, channelName), user_id);
		else
			send_msg(ERR_NOSUCHNICK(invNick), user_id);
	}
}