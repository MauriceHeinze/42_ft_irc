#ifndef SERVER_HPP
# define SERVER_HPP

#include "Utils.hpp"

class Server
{
	public:

		Server();
		Server(char **argv);
		~Server();

		void setSocket();
		void startServer();
		void acceptConnection(size_t i);
		void recvMsg(size_t i);

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

		int	_socket;
		int	_port;
		std::string _password;

		std::vector<pollfd> _fds;

};

#endif