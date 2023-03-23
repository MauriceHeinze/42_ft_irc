#include "./Headers/Channel.hpp"

Channel::Channel()
{
	_settings->inviteOnly = false;
	_settings->moderated = false;
	_settings->msgFromOutside = true;
	_settings->password = "";
	_settings->privateChannel = false;
	_settings->secretChannel = false;
	_settings->topicOperatorOnly = false;
	_settings->userLimit = -1;
}

// Channel::Channel(const Channel &a)
// {

// }

Channel::~Channel()
{

}

// Channel& Channel::operator= (const Channel& a)
// {

// 	return (*(this));
// }

void	Channel::join(User &userRef)
{
	permissions perm = {false, false, true, &userRef};
	_perm.push_back(perm);
}

void	Channel::part(std::string nickname)
{
	for (size_t i = 0; i < _perm.size(); i++)
	{
		if (_perm[i].name->getNickname() == nickname)
		{
			_perm.erase(_perm.begin() + i);
			_perm.shrink_to_fit();
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
			_perm.shrink_to_fit();
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

void	Channel::setTopic( std::string topic )
{
	this->_topic = topic;
}

std::string	Channel::getTopic( void )
{
	return (this->_topic);
}



// Funktion schreiben die Rechte zurückgibt, damit Lars weiss, wann er Funktionen aufrufen kann
