
#ifndef CLASS_User
# define CLASS_User

class User
{
	private:

	public:
		User();
		User(const User &a);
		~User();
		User &operator= (const User& a);
};

#endif

/*

#include	"User.hpp"

User::User()
{
	
}

User::User(const User &a)
{
	
}

User::~User()
{
	
}

User& User::operator= (const User& a)
{
	
	return (*(this));
}
*/