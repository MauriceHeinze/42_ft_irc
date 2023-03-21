
#ifndef CLASS_Connection
# define CLASS_Connection

# include	<string>
# include <sys/poll.h>


class Connection
{
	private:
		std::string	_password;
	public:
		bool		_valid_password;
		Connection();
		// Connection(const Connection &a);
		~Connection();
		// Connection &operator= (const Connection& a);
		void		set_password(std::string);
		std::string	get_password();
};

#endif
