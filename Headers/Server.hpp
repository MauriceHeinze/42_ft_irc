#ifndef SERVER_HPP
# define SERVER_HPP

#include "Utils.hpp"
#include "Channel.hpp"
#include "User.hpp"

class Server
{
	public:

		Server();
		Server(char **argv);
		~Server();

		// std::vector<Channel> _channels;

		void 		setSocket();
		void 		startServer();
		void 		acceptConnection();
		void 		recvMsg(size_t i);
		void		Command_PASS( std::string , int iter);
		std::string	get_password( void );

		class SocketCreationFail: public std::exception {
			const char *what() const throw() {
				return "Error: could not set the socket";
			}
		};

		class SocketBindingFail: public std::exception {
			const char *what() const throw() {
				return "Error: could not bind the socket";
			}
		};

		class SocketListenFail: public std::exception {
			const char *what() const throw() {
				return "Error: could not listen to socket";
			}
		};

		class ClientAcceptFail: public std::exception {
			const char *what() const throw() {
				return "Error: could not accept the client";
			}
		};

		class PollFail: public std::exception {
			const char *what() const throw() {
				return "Error: poll failed";
			}
		};
	private:
		void	parsing(std::string, int iter);
		int	_socket;
		int	_port;
		std::string _password;
		// std::vector<Channel> _channels;
		std::vector<User> _users;
		std::vector<pollfd> _fds;
		//std::vector<Connection> _con;
};

#endif