
#ifndef CLASS_Connection
# define CLASS_Connection

# include	<string>
# include <sys/poll.h>
# include "User.hpp"

class Connection
{
	private:
	public:
		pollfd*		_pollfd;
		bool		_valid_password;
		Connection( pollfd* );
		~Connection();
};

#endif
