#include "./Utils.hpp"

#ifndef CLASS_User
# define CLASS_User

class User
{
	private:
		std::string	username;
		std::string	fullName;
		std::string	nickname;
	public:
		std::string			msg; // buffer to store message content until \r\n is received // check recv function options for buffer
		User( std::string username );
		User(const User &a);
		~User();
		User				&operator= (const User& a);
		void				setNickname( const std::string &name);
		const std::string	&getNickname( void );
		const std::string	&getUsername( void );
		const std::string	&getFullname( void );
};

#endif