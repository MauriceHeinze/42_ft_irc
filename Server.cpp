#include 	"./Headers/Server.hpp"
#include	<sstream>
#include	"./Headers/Commands.hpp"
#include	"./Headers/User.hpp"

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
	User first_one;
	 first_one._pollfd->fd = fd.fd;
	_users.push_back(first_one);
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
			if (_fds[i].revents & (POLLHUP | POLL_ERR | POLLNVAL))
			{
				close(_fds[i].fd);
				std::cout << " Closing this fd = " <<_fds[i].fd << std::endl;
				_fds.erase(_fds.begin() + i);
				_fds.shrink_to_fit();
				_users.erase(_users.begin() + i);
				_users.shrink_to_fit();
			}
			if (_fds[i].revents & POLLIN) {
				if (_fds[i].fd == _socket) {
					acceptConnection();
				}
				else
				{
					std::cout << "MSG recieve from socket " << i << std::endl;
					recvMsg(i);
				}
			}
		}
	}
	close( _socket);
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
		std::cout << "Client connected!" << "using the fd =" << clientSocket <<std::endl;
	pollfd new_fd = {clientSocket, POLLIN, 0};
	_fds.push_back(new_fd);
	User new_user;
	_users.push_back(new_user);
	const std::string msg = ":Server opening :Hallo, was geht\r\n";
	send(clientSocket, msg.c_str(), msg.size(), 0);
}

#include	"./Headers/BNF_Translate.hpp"

void	Server::parsing(std::string buffer, int iter)
{
	(void)iter;// just for Error flags
	for (size_t i = 0; i < buffer.size(); i++)
	{
		std::cout << (int)buffer[i] << std::endl;
	}
	BNF_Translate msg(buffer);
	if (msg.getter_command() == "CAP")
	{
		std::cout << "Cap send" << std::endl;
		send(this->_fds[iter].fd,":irc.unknown.net CAP * LS :\13\10",30,0);// no capabilities
		send(this->_fds[iter].fd,"PONG",4,0);// temp
	}
	if (msg.getter_command() == "PIG")
	{
		BNF_Translate send_msg(msg.get_full_msg());
		send_msg.setter_command("PONG");
		send(this->_fds[iter].fd,send_msg.get_full_msg().c_str(),send_msg.get_full_msg().size(),0);
	}
	if (msg.getter_command() == "PASS")
	{
		// call PASS_func
	}
	// protection for everthing that need Password_valid
	if (msg.getter_command() == "NICK")
	{
		//call Nick_func	
	}
	// protection for everthing that need valid_nick

}

void Server::recvMsg(size_t i)
{

	char buffer[1024];
	int valread;
	valread = recv(_fds[i].fd, buffer, 1024, 0);
	if (valread == 0)
	{
		std::cout << "Connection Closed " << i << std::endl;
		return ;
	}	
	if (buffer[0] != 0)
		std::cout << "> " << buffer << std::endl;
	parsing(buffer, i);
	// const std::string msg = ":Server respond\r\n";
	// send(_fds[i].fd, msg.c_str(), msg.size(), 0);
	buffer[0] = 0;
	_fds[i].revents = 0;
	_fds[i].events = POLLIN;
}

std::string	Server::get_password( void )
{
	return(_password);
}