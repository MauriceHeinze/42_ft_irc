#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Utils.hpp"
#include "User.hpp"

struct permissions{
	bool	isAdmin;
	bool	isVoice;
	bool	isAllowedToSpeak;
	User	*name;
};

struct channelSettings{
	bool			privateChannel;
	bool			secretChannel;
	bool			inviteOnly;
	bool			topicOperatorOnly;
	bool			msgFromOutside;
	bool			moderated;
	unsigned int	userLimit;
	std::string		password;
};

class Channel
{
	private:
		std::string	_topic;
		std::vector<std::string> _bannedUsers;
		std::vector<permissions> _perm;
	public:
		Channel();
		Channel(const Channel &a);
		~Channel();
		Channel &operator= (const Channel& a);
		channelSettings	_settings;

		void		setTopic( std::string topic );
		std::string	getTopic( void );

		void 		join(User &userRef, std::string password);
		void 		part(std::string username);
		void 		kick(std::string adminName, std::string username);
		void 		oper(std::string adminName, std::string username);
		void 		mode(std::string adminName, std::string username);
};

#endif