
#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP

#include	"Utils.hpp"
#include	"TranslateBNF.hpp"

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
		void	Command_PASS( TranslateBNF msg, int user_id);
		void	Command_JOIN( TranslateBNF msg, int user_id);
		void	Command_KICK( TranslateBNF msg, int user_id);
		void	Command_TOPIC(TranslateBNF msg, int user_id);
		void	Command_NICK( TranslateBNF msg, int user_id);
		void	Command_PART( TranslateBNF msg, int user_id);
		void	Command_MODE( TranslateBNF msg, int user_id);
		void	Command_P_MSG(TranslateBNF msg, int user_id);
		void	Command_PING( TranslateBNF msg, int user_id);
		void	Command_CAP(  TranslateBNF msg, int user_id);
		void	send_msg(std::string msg, int user_id);
		void	parsing( std::string msg, int user_id);
		int	_socket;
		int	_port;
		std::string _password;
		std::vector<Channel> _channels;
		std::vector<User> _users;
		std::vector<pollfd> _fds;
		//std::vector<Connection> _con;
};

#endif