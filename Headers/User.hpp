#include "./Utils.hpp"

#ifndef CLASS_User
# define CLASS_User

class User
{
	private:
		const std::string	username;
		const std::string	nickname;
	public:
		User::User( std::string username, std::string nickname );
		User(const User &a);
		~User();
		User &operator= (const User& a);
		const std::string getUsername( void );
		const std::string getNickname( void );
};

#endif