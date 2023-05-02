#pragma once
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
		std::string					_topic;
		std::vector<permissions> 	_perm;//? better as <user>
		std::vector<std::string> 	_invited;//? better as <user>
		std::string					_name;
	public:
		channelSettings				_settings;
		Channel(std::string name);
		Channel(std::string name, std::string password);
		Channel(std::string name, std::string password, User& first_user);
		Channel(const Channel &a);
		~Channel();
		Channel &operator= (const Channel& a);
		//getter and setter
		void		setTopic( std::string nickname, std::string topic );
		std::string	getTopic( void );
		std::string	getName( void );
		//Utiels
		void		send_to_all(std::string msg);
		//Channel commands
		void		join(User &userRef);
		int			add_new_user(User& user, std::string channel_password);// user& , return error/ string/code
		void		part(std::string nickname);
		void		kick(std::string nickname);
		void		oper(std::string nickname);
		void		mode(std::string nickname);
		void		invite(std::string nickname);
		//check Channel settings
		bool		isInvited(std::string nickname);
		bool		isAdmin(std::string nickname);
		bool		isUser(std::string nickname);
		bool		isVoice(std::string nickname);
		bool		isAllowedToSpeak(std::string nickname);
		bool		checkLimit();
		bool		userExists(std::string nickname);
};

#endif


// CAP LS -
// CAP * LS :End of cap negotation //
// CAP END

// PING