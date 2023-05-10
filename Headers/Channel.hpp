#pragma once
#ifndef CHANNEL_HPP
# define CHANNEL_HPP
#include "User.hpp"

#include "Utils.hpp"

struct permissions{
		bool	isAdmin;
		bool	isVoice;
		bool	isAllowedToSpeak;
		User	*user;
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
		std::vector<permissions> 	_perm;
		std::vector<std::string> 	_invited;//? better as <user>
		std::string					_name;
	public:
		channelSettings				_settings;//!should be in privat
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
		void		send_to_not_all(std::string msg, int not_this_fd);
		void		leave_user(User* user, std::string msg);
		std::string	get_user_list( void );
		//Channel commands
		void		join(User &userRef);
		int			add_new_user(User& user, std::string channel_password);// user& , return error/ string/code
		void		part(std::string nickname);
		void		kick(std::string nickname);
		void		oper(std::string nickname);
		void		mode(std::string nickname);
		void		invite(std::string nickname);
		//check Channel settings
		bool		isInvited(std::string nickname);// use user instead of string saves you the getter of the nickname all the time
		bool		isAdmin(std::string nickname);
		bool		isVoice(std::string nickname);
		bool		isAllowedToSpeak(std::string nickname);
		bool		checkLimit();
		size_t		find_user_in_channel(User* user);
		// bool		isUser(std::string nickname);
		bool		userExists(std::string nickname);
		std::string	getSettings(void);
};

#endif


// CAP LS -
// CAP * LS :End of cap negotation //
// CAP END

// PING