
#ifndef CLASS_Connection
# define CLASS_Connection

# include	<string>
# include <sys/poll.h>


class Connection
{
	private:
		std::string	_password;
	public:
		pollfd*		_pollfd;
		bool		_valid_password;
		Connection( pollfd* );
		~Connection();
		void		set_password(std::string);
		std::string	get_password();
};

#endif
