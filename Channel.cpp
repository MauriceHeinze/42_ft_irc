#include "./Headers/Channel.hpp"

Channel::Channel()
{

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

void Channel::join(User &userRef)
{
	permissions perm = {false, false, &userRef};
	_perm.push_back(perm);
}

void Channel::part(std::string username)
{
	for (size_t i = 0; i < _perm.size(); i++)
	{
		if (_perm[i].name->getUsername() == username)
		{
			_perm.erase(_perm.begin() + i);
			_perm.shrink_to_fit();
			return ;
		}
	}
	throw("User not found in this channel");
}

void Channel::kick(std::string adminName, std::string username)
{
	// find admin
	for (size_t i = 0; i < _perm.size(); i++)
	{
		if (_perm[i].name->getUsername() == adminName)
		{
			// check for permissions
			if (!_perm[i].isAdmin)
			{
				throw("User without permission tries to kick");
				return ;
			}
			// check for user to kick
			for (size_t k = 0; k < _perm.size(); k++)
			{
				if (_perm[k].name->getUsername() == username)
				{
					_perm.erase(_perm.begin() + k);
					_perm.shrink_to_fit();
					return ;
				}
			}
			throw("User to kick not found in this channel");
		}
	}
	throw("Admin not found in this channel");
}

void Channel::oper(std::string adminName, std::string username)
{
	// find admin
	for (size_t i = 0; i < _perm.size(); i++)
	{
		if (_perm[i].name->getUsername() == adminName)
		{
			// check for permissions
			if (!_perm[i].isAdmin)
			{
				throw("User without permission tries to promote someone to operator");
				return ;
			}
			// check for user to promote to operator
			for (size_t k = 0; k < _perm.size(); k++)
			{
				if (_perm[k].name->getUsername() == username)
				{
					_perm[k].isAdmin = true;
					_perm[k].isVoice = true;
					return ;
				}
			}
			throw("User to promote not found in this channel");
		}
	}
	throw("Admin that wants to promote not found in this channel");
}

void	Channel::setTopic( std::string topic )
{
	this->_topic = topic;
}

std::string	Channel::getTopic( void )
{
	return (this->_topic);
}