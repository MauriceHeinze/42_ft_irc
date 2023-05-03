#pragma once
#ifndef CLASS_User
# define CLASS_User

#include "Utils.hpp"

class User 
{
	private:
		std::string			username;
		std::string			fullName;
		std::string			nickname;
	public:
		const int			_fd;//aka socket
		bool				_valid_password;
		bool				_valid_nickname;
		std::string			msg; // buffer to store message content until \r\n is received // check recv function options for buffer
		User( std::string username,int fd);
		User(int fd);
		// User(const User &a);
		~User();
		void				insert_in_user_buffer(std::string read_buffer);
		std::string   		get_next_command( void );
		User				&operator= (const User& a);
		void				setNickname( const std::string &name);
		const std::string	&getNickname( void );
		void				setUsername( const std::string &name);
		const std::string	&getUsername( void );
		void				setFullname( const std::string &name);
		const std::string	&getFullname( void );
};

#endif