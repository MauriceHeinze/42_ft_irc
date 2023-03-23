#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Utils.hpp"
#include "Server.hpp"
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
		std::vector<permissions> _perm;
		std::vector<std::string> _invited;
		// std::vector<User> *_allServerUsers;

	public:
		Channel();
		Channel(const Channel &a);
		~Channel();
		Channel &operator= (const Channel& a);
		channelSettings	*_settings;

		void		setTopic( std::string topic );
		std::string	getTopic( void );

		void		join(User &userRef);
		void		part(std::string nickname);
		void		kick(std::string nickname);
		void		oper(std::string nickname);
		void		mode(std::string nickname);
		void		invite(std::string nickname);

		bool		isInvited(std::string nickname);

};

#endif

// CAP LS -
// CAP * LS :End of cap negotation //
// CAP END

// PING