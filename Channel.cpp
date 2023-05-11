#include "./Headers/Utils.hpp"

#ifndef UINT_MAX
 # define UINT_MAX 500
#endif

#define SEND_FLAGS 0

Channel::Channel(std::string name) :_name(name)
{
	out(name + " Channel created")
	_settings.inviteOnly = false;
	_settings.moderated = false;
	_settings.msgFromOutside = true;
	_settings.password = "";
	_settings.privateChannel = false;
	_settings.secretChannel = false;
	_settings.topicOperatorOnly = false;
	_settings.userLimit = UINT_MAX;
}

Channel::Channel(std::string name, std::string password) :_name(name)
{
	out(name + " Channel created")
	_settings.inviteOnly = false;
	_settings.moderated = false;
	_settings.msgFromOutside = true;
	_settings.password = password;
	_settings.privateChannel = false;
	_settings.secretChannel = false;
	_settings.topicOperatorOnly = false;
	_settings.userLimit = UINT_MAX;
}

Channel::Channel(std::string name, std::string password, User& first_user) :_name(name)
{
	out(name + " Channel created")
	_settings.inviteOnly = false;
	_settings.moderated = false;
	_settings.msgFromOutside = true;
	_settings.password = password;
	_settings.privateChannel = false;
	_settings.secretChannel = false;
	_settings.topicOperatorOnly = false;
	_settings.userLimit = UINT_MAX;
	out("before")
	out(_perm.size())
	this->add_new_user(first_user, password);
	out(_perm.size())
	this->_perm[0].isAdmin = true;
	out("after")
}

Channel::Channel(const Channel &a)
{
	*this = a;
}

Channel::~Channel()
{

}

Channel& Channel::operator= (const Channel &other)
{
	this->_name = other._name;
	this->_topic = other._topic;
	this->_perm = other._perm;
	this->_invited = other._invited;
	this->_settings = other._settings;
	return(*this);
}

void	Channel::join(User &userRef)
{
	out("in join????")
	permissions perm = {false, false, true, &userRef};
	_perm.push_back(perm);
	out(_perm.size())
}

bool	Channel::checkLimit(){
	if (_perm.size() < _settings.userLimit)
		return true;
	return false;
}

void	Channel::part(std::string nickname)
{
	for (size_t i = 0; i < _perm.size(); i++)
	{
		if (_perm[i].user->getNickname() == nickname)
		{
			_perm.erase(_perm.begin() + i);
			// _perm.shrink_to_fit();
			return ;
		}
	}
	throw("User not found in this channel");
}

void	Channel::leave_user(User* user ,std::string msg)
{
	(void)msg;
	for (size_t i = 0; i < _perm.size(); i++)
	{
		if (user == _perm[i].user)
		{
			_perm.erase(_perm.begin() + i);
			//send part msg reason disconecet
			break;
		}
	}

}

void	Channel::kick(std::string nickname)
{
	// check for user to kick
	for (size_t k = 0; k < _perm.size(); k++)
	{
		if (_perm[k].user->getNickname() == nickname)
		{
			_perm.erase(_perm.begin() + k);
			return ;
		}
	}
	throw("User to kick not found in this channel");
}

void	Channel::oper(std::string nickname)
{
	// check for user to promote to operator
	for (size_t k = 0; k < _perm.size(); k++)
	{
		if (_perm[k].user->getNickname() == nickname)
		{
			_perm[k].isAdmin = true;
			_perm[k].isVoice = true;
			return ;
		}
	}
	throw("User to promote not found in this channel");
}

void	Channel::invite(std::string nickname)
{
	// check if user joined already
	for (size_t i = 0; i < _perm.size(); i++)
	{
		if (_perm[i].user->getNickname() == nickname)
		{
			throw("User is already member of channel");
			return ;
		}
	}
	// check if user got invited already
	for (size_t i = 0; i < _invited.size(); i++)
	{
		if (_invited[i] == nickname)
		{
			throw("User already invited");
			return ;
		}
	}
	// invite
	_invited.push_back(nickname);
}

void	Channel::setTopic( std::string nickname, std::string topic )
{
	if (_settings.topicOperatorOnly)
	{
		if (isAdmin(nickname))
			this->_topic = topic;
	}
	else
		this->_topic = topic;
}

std::string	Channel::getTopic( void )
{
	return (this->_topic);
}

std::string	Channel::getName( void )
{
	return (this->_name);
}

bool	Channel::isInvited(std::string nickname){
	for(size_t i = 0; i < _invited.size(); i++)
	{
		if (_invited[i] == nickname)
			return true;
	}
	return false;
}

bool	Channel::isAdmin(std::string nickname){
	out(_perm.size())
	for(size_t i = 0; i < _perm.size(); i++)
	{
		out(i)
		if (_perm[i].user->getNickname() == nickname){
			if (_perm[i].isAdmin == true)
				return true;
			else
				return false;
		}
	}
	return false;
}

// i: Set/remove Invite-only channel
// · t: Set/remove the restrictions of the TOPIC command to channel
// operators
// · k: Set/remove the channel key (password)
// · o: Give/take channel operator privilege
// 5
// ft_irc Internet Relay Chat
// · l: Set/remove the user limit to channel

std::string	Channel::getSettings(void) {
	std::string	settings = "";

	if (_settings.userLimit < INT_MAX || _settings.inviteOnly || _settings.topicOperatorOnly
		|| _settings.password.length() > 0 || _settings.moderated)
		settings.append("+");
	if (_settings.inviteOnly)
		settings.append("i");
	if (_settings.topicOperatorOnly)
		settings.append("t");
	if (_settings.password.length() > 0)
		settings.append("k");
	if (_settings.moderated)
		settings.append("o");
	if (_settings.userLimit < INT_MAX)
		settings.append("l");

	if (_settings.password.length() > 0)
		settings.append(" " + _settings.password);
	if (_settings.userLimit < INT_MAX)
		settings.append(" " + std::to_string(_settings.userLimit));
	return (settings);
}

bool	Channel::userExists(std::string nickname){
	for(size_t i = 0; i < _perm.size(); i++)
	{
		if (_perm[i].user->getNickname() == nickname)
				return true;
	}
	return false;
}

size_t	Channel::find_user_in_channel(User* user)
{
	for (size_t i = 0; i < _perm.size(); i++)
	{
		if (user == _perm[i].user)
			return(i);
	}
	return(USER_NOT_FOUND);
}

bool	Channel::isVoice(std::string nickname){
	for(size_t i = 0; i < _perm.size(); i++)
		{
			if (_perm[i].user->getNickname() == nickname)
			{
				if (_perm[i].isVoice == true)
					return true;
				else
					return false;
			}
	}
	return false;
}

// Try to add User to the Channel and returns a reply code if failed or not
int	Channel::add_new_user(User& user, std::string used_password)// user& , return error/ string/code
{
	if ( this->find_user_in_channel(&user) == USER_NOT_FOUND)
	{
		// check if user needs/is invited
		if (this->_settings.inviteOnly && this->isInvited(user.getNickname()) == false)
		{
			return (rpl_ERR_INVITEONLYCHAN);
		}
		// check if password for channel is correct/required
		if (this->_settings.password.length() > 0 && this->_settings.password != used_password)
		{
			return (rpl_ERR_BADCHANNELKEY);
		}
		this->join(user);
		return (rpl_default);
	}
	return (-1);
	out("failed check")
}

bool	Channel::isAllowedToSpeak(std::string nickname){
	for(size_t i = 0; i < _perm.size(); i++)
	{
		if (_perm[i].user->getNickname() == nickname){
			if (_perm[i].isAllowedToSpeak == true)
				return true;
			else
				return false;
		}
	}
	return false;
}


void	Channel::send_to_all(std::string msg)
{
	out("\e[31m" + msg + "\e[0m")
	out(_perm.size())

	for (size_t i = 0; i < _perm.size(); i++){
		out(_perm[i].user->_fd)
		send(_perm[i].user->_fd, msg.c_str(), msg.size(), SEND_FLAGS);
	}
}

void	Channel::send_to_not_all(std::string msg, int not_this_fd)
{
	out("\e[31m" + msg + "\e[0m")
	for (size_t i = 0; i < _perm.size(); i++)
	{
		if (_perm[i].user->_fd != not_this_fd)
			send(_perm[i].user->_fd, msg.c_str(), msg.size(), SEND_FLAGS);
	}
}

std::string	Channel::get_user_list( void )
{
	std::string list;

	out(_perm.size())
	for (size_t i = 0; i < _perm.size(); i++)
	{
		out("test")
		if (i != 0)
			list.append(" ");
		else
			list.append(":");
		if (_perm[i].isAdmin == true)
			list.append("@");
		if (_perm[i].isVoice == true)//not sure if this is correct
			list.append("+");
		list.append(_perm[i].user->getNickname());
	}
	out("list")
	out(list)
	return(list);
}