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

void	Channel::setTopic( std::string topic )
{
	this->topic = topic;
}

std::string	Channel::getTopic( void )
{
	return (this->topic);
}