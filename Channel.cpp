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
	this->add_new_user(first_user, "");
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
		if (_perm[i].name->getNickname() == nickname)
		{
			_perm.erase(_perm.begin() + i);
			// _perm.shrink_to_fit();
			return ;
		}
	}
	throw("User not found in this channel");
}

void	Channel::kick(std::string nickname)
{
	// check for user to kick
	for (size_t k = 0; k < _perm.size(); k++)
	{
		if (_perm[k].name->getNickname() == nickname)
		{
			_perm.erase(_perm.begin() + k);
			// _perm.shrink_to_fit();
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
		if (_perm[k].name->getNickname() == nickname)
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
		if (_perm[i].name->getNickname() == nickname)
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
	out("getName")
	out(this->_name)
	out("----")
	return (this->_name);
}

bool	Channel::isInvited(std::string nickname){
	for(int i = 0; _invited.size(); i++)
	{
		if (_invited[i] == nickname)
			return true;
	}
	return false;
}

bool	Channel::isAdmin(std::string nickname){
	for(int i = 0; _perm.size(); i++)
	{
		if (_perm[i].name->getNickname() == nickname){
			if (_perm[i].isAdmin == true)
				return true;
			else
				return false;
		}
	}
	return false;
}

bool	Channel::isUser(std::string nickname){
	for(int i = 0; _perm.size(); i++)
	{
		if (_perm[i].name->getNickname() == nickname)
			return true;
	}
	return false;
}

bool	Channel::isVoice(std::string nickname){
	for(int i = 0; _perm.size(); i++)
		{
			if (_perm[i].name->getNickname() == nickname)
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
	if ( this->userExists(user.getNickname()) == false)
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
}

bool	Channel::isAllowedToSpeak(std::string nickname){
	for(int i = 0; _perm.size(); i++)
	{
		if (_perm[i].name->getNickname() == nickname){
			if (_perm[i].isAllowedToSpeak == true)
				return true;
			else
				return false;
		}
	}
	return false;
}

bool	Channel::userExists(std::string nickname){
	for(int i = 0; _perm.size(); i++)
	{
		if (_perm[i].name->getNickname() == nickname)
				return true;
	}
	return false;
}

void	Channel::send_to_all(std::string msg)
{
	out("perm size")
	out(_perm.size())
	for (size_t i = 0; i < _perm.size(); i++)
	{
		out("user >>")
		out(_perm[i].name->getNickname())
		send(_perm[i].name->fd, msg.c_str(), msg.size(), SEND_FLAGS);
	}
}