#include "./Utils.hpp"

#ifndef CLASS_User
# define CLASS_User

class User
{
	private:
		const	std::string	username;
		std::string			nickname;
	public:
		User( std::string username );
		User(const User &a);
		~User();
		User &operator= (const User& a);
		void				setNickname( std::string name);
		std::string			getNickname( void );
		const std::string	getUsername( void );
};

#endif