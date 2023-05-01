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
#define USER_LIST(nickname,channel,user1,user2) "353 " + nickname +" " + channel + " :" + user1 + " " + user2 + "test_fail" + "\r\n"
#define RPL_ENDOFNAMES(nickname, channel) "366 " + nickname + " " + channel + "\r\n"
#define RPL_JOIN(nickname, channel) ":" + nickname + " JOIN :" + channel + "\r\n"
//new channel
 void Server::create_new_channel(std::string new_channel ,int user_id, std::string channel_password)
{
	//!check for valid channel name
	
	//create the new channel
	Channel Channel(new_channel, channel_password);
	this->_channels.push_back(Channel);
	//!give first user adminrights
	//send list of user and topics if succesfull
	this->send_msg(RPL_JOIN(_users[user_id].getNickname(), new_channel), user_id);
	out("in new")
	out(Channel.getName())
}


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
		out("Nice")
		//to all channel members
		this->send_msg(":lkrabbe JOIN :abc\r\n",user_id);
	}
	else if (rpl_msg == rpl_no_rpl)
		return;
	else
		out("error missing !!!!(use_old_channel)")
	//send list of user and topics if succesfull
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
	out("in join")
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
	if (msg.getter_params().size() > 0)
	{
		std::string channelName = msg.getter_params()[0].trailing_or_middle;
		if (isChannel(_channels, channelName))
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

	std::string channelName = msg.getter_params()[0].trailing_or_middle;
	std::string nickname = msg.getter_params()[1].trailing_or_middle;
	std::string nicknameToBeKicked = msg.getter_params()[1].trailing_or_middle;
	std::string comment = msg.getter_params()[2].trailing_or_middle;
	int channelIndex = this->find_Channel(channelName);
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
			this->send_msg(ERR_CHANOPRIVSNEEDED(nickname, currentChannel->getName()), user_id);
	}
	else if (channelIndex != -1)
	{
		this->send_msg(ERR_NOSUCHCHANNEL(nickname, currentChannel->getName()), user_id);
	}
	else if (!userToBeKickedExists)
		this->send_msg(ERR_NOTONCHANNEL(nickname, currentChannel->getName()), user_id);
}

void	Server::Command_TOPIC(TranslateBNF msg,int user_id)
{
	(void)user_id;
	std::string channelName = msg.getter_params()[0].trailing_or_middle;
	std::string nickname = msg.getter_params()[1].trailing_or_middle;
	std::string topic = msg.getter_params()[2].trailing_or_middle;
	int channelIndex = this->find_Channel(channelName);
	bool userExists = this->_channels[channelIndex].userExists(nickname);
	Channel	*currentChannel = &this->_channels[channelIndex];

	if (channelIndex != -1 && userExists)
	{
		if (topic.length() == 0)
		{
			if (currentChannel->getTopic().length() > 0)
				this->send_msg(RPL_TOPIC(nickname, currentChannel->getName(), currentChannel->getTopic()), user_id);
			else
				this->send_msg(RPL_NOTOPIC(nickname, currentChannel->getName()), user_id);
		}
		else
		{
			if (currentChannel->isAdmin(nickname))
			{
				currentChannel->setTopic(nickname, topic);
				this->send_msg(RPL_TOPIC(nickname, currentChannel->getName(), currentChannel->getTopic()), user_id);
			}
			else
				this->send_msg(ERR_CHANOPRIVSNEEDED(nickname, currentChannel->getName()), user_id);
		}
	}
	else if (!userExists)
		this->send_msg(ERR_NOTONCHANNEL(nickname, currentChannel->getName()), user_id);
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
	std::string channelName = msg.getter_params()[0].trailing_or_middle;
	std::string nickname = msg.getter_params()[1].trailing_or_middle;
	int channelIndex = this->find_Channel(channelName);
	bool userExists = this->_channels[channelIndex].userExists(nickname);
	Channel	*currentChannel = &this->_channels[channelIndex];
	if (channelIndex != -1 && userExists)
		currentChannel->part(nickname);
	else if (channelName.length() == 0)
		this->send_msg(ERR_NEEDMOREPARAMS(nickname, msg.getter_command()), user_id);
	else if (channelIndex == -1)
		this->send_msg(ERR_NOSUCHCHANNEL(nickname, currentChannel->getName()), user_id);
	else if (!userExists)
		this->send_msg(ERR_NOTONCHANNEL(nickname, currentChannel->getName()), user_id);
}

void	Server::Command_P_MSG(TranslateBNF msg, int user_id)
{
	if (msg.getter_params().size() > 0)
	{
		if (msg.getter_params()[0].trailing_or_middle.find("#") != std::string::npos){
			// if (isChannel(_channels, msg.getter_params()[0].trailing_or_middle)){
				send_msg(":" + _users[user_id].getNickname() + " PRIVMSG " + msg.getter_params()[0].trailing_or_middle + " :" + msg.getter_params()[1].trailing_or_middle + "\r\n", user_id);
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

void	Server::Command_MODE(TranslateBNF msg, int user_id)
{
	(void)user_id;
	(void)msg;
}

void	Server::Command_PING(TranslateBNF msg, int user_id)
{
	TranslateBNF send_msg(msg.get_full_msg());
	send_msg.setter_command("PONG");
	// send_msg("PONG\13\10", user_id);
	send(this->_fds[user_id].fd,send_msg.get_full_msg().c_str(),send_msg.get_full_msg().size(),0);
}

void	Server::Command_CAP(TranslateBNF msg, int user_id)
{
	(void)msg;
	std::cout << "Cap send" << std::endl;
	send_msg(":irc.unknown.net CAP * LS :\r\n", user_id);
}
