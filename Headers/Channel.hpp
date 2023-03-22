#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Utils.hpp"

class Channel
{
	private:
		std::string	topic;
	public:
		Channel();
		Channel(const Channel &a);
		~Channel();
		Channel &operator= (const Channel& a);
		void		setTopic( std::string topic );
		std::string	getTopic( void );
};

#endif