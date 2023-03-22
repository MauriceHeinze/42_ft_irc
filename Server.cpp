#include 	"./Headers/Server.hpp"
#include	<sstream>
#include	"./Headers/commands.h"
#include	"./Headers/Connection.hpp"

Server::Server(){
	return ;
}

Server::Server(char **argv)
{
	_port = atoi(argv[1]);
	_password = argv[2];
}

Server::~Server(){
	return ;
}

void Server::setSocket(){
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(_socket, F_SETFL, O_NONBLOCK);
	if(_socket == -1)
		throw(SocketCreationFail());
	int optval = 1;
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1){
		std::cerr << "setsockopt failed" << std::endl; 
		return ;
	}
	sockaddr_in servAddress;
	servAddress.sin_family = AF_INET;
	servAddress.sin_addr.s_addr = INADDR_ANY;
	servAddress.sin_port = htons(_port);
	if (bind(_socket, (sockaddr *)&servAddress, sizeof(servAddress)) == -1)
		throw(SocketBindingFail());
	if (listen(_socket, 20) == -1)
		throw(SocketListenFail());
	pollfd fd = {_socket, POLLIN, 0};
	_fds.push_back(fd);
	Connection first_one(&_fds.back());
	std::cout << " pollfd " << fd.fd ;
	std::cout << "con " << first_one._pollfd->fd << std::endl;
	_con.push_back(first_one);
}

void Server::startServer(){
	try {
		setSocket();
	}
	catch(std::exception& e) {
		std::cerr << e.what() << '\n';
		return ;
	}

	while (1) {
		int pollVal = poll(_fds.data(), _fds.size(), 5000);
		std::cout << "current connections " << _fds.size() - 1 << std::endl;
		if (pollVal == -1)
			throw(PollFail());
		for (size_t i = 0; i < _fds.size(); i++) {
			if (_fds[i].revents & (POLLHUP | POLL_ERR | POLLNVAL)) {
				_fds.erase(_fds.begin() + i);
				_fds.shrink_to_fit();
				_con.erase(_con.begin() + i);
				_con.shrink_to_fit();
			}	
			if (_fds[i].revents & POLLIN) {
				if (_fds[i].fd == _socket) {
					acceptConnection();
				}
				else
					recvMsg(i);
			}
		}
	}
	close(_socket);
}

void Server::acceptConnection()
{
	sockaddr_in clientAddress;
	socklen_t clientAddressSize = sizeof(clientAddress);
	int clientSocket = accept(_socket, (sockaddr *)&clientAddress, &clientAddressSize);
	fcntl(clientSocket, F_SETFL, O_NONBLOCK);
	if (clientSocket ==  -1)
		throw(ClientAcceptFail());
	else
		std::cout << "Client connected!" << std::endl;
	pollfd new_fd = {clientSocket, POLLIN, 0};
	_fds.push_back(new_fd);
	Connection new_con(&_fds.back());
	_con.push_back(new_con);
	const std::string msg = ":Server opening Hallo, was geht\r\n";
	send(clientSocket, msg.c_str(), msg.size(), 0);
}

void	Server::parsing(std::string buffer, Connection& con)
{
	std::string input(buffer,0,buffer.find_first_of(32));
	buffer.erase(0,buffer.find_first_of(32));
	// std::cout << buffer.find_first_of(32) << std::endl;
	// while()
	// {
		// input.insert(buffer.find_first_of(32),buffer);
	// std::cout << input  << "|" << std::endl;
	// }
	if ( input == "PASS" )
	 {
		Command_PASS(con, buffer);
	}
}


void Server::recvMsg(size_t i)
{
	char buffer[1024] = {0};
	int valread;
	valread = recv(_fds[i].fd, buffer, 1024, 0);
	if (buffer[0] != 0)
		std::cout << "> " << buffer << std::endl;
	parsing(buffer, _con[i]);
	// const std::string msg = ":Server respond\r\n";
	// send(_fds[i].fd, msg.c_str(), msg.size(), 0);
	buffer[0] = 0;
	_fds[i].revents = 0;
	_fds[i].events = POLLIN;
}

std::string	Server::get_password( void )
{
	return(_password);
};