#pragma once
#ifndef CLASS_User
# define CLASS_User

#include "Utils.hpp"

class User : public pollfd
{
	private:
		std::string	username;
		std::string	fullName;
		std::string	nickname;
	public:
		pollfd*		_pollfd;
		bool		_valid_password;
		bool		_valid_nickname;
		std::string			msg; // buffer to store message content until \r\n is received // check recv function options for buffer
		User( std::string username);
		User();
		// User(const User &a);
		~User();
		User				&operator= (const User& a);
		void				setNickname( const std::string &name);
		const std::string	&getNickname( void );
		void				setUsername( const std::string &name);
		const std::string	&getUsername( void );
		void				setFullname( const std::string &name);
		const std::string	&getFullname( void );
};

#endif