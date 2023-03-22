#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Utils.hpp"
#include "User.hpp"

struct permissions{
	bool isAdmin;
	bool isVoice;
	User *name;
};

class Channel
{
	private:
		std::string	_topic;
		std::vector<permissions> _perm;
	public:
		Channel();
		Channel(const Channel &a);
		~Channel();
		Channel &operator= (const Channel& a);
		void		setTopic( std::string topic );
		std::string	getTopic( void );
		void join(User &userRef);
		void part(std::string username);
		void kick(std::string adminName, std::string username);
		void oper(std::string adminName, std::string username);
		void mode(std::string adminName, std::string username);
};

#endif