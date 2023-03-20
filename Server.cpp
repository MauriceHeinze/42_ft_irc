#include "./Headers/Server.hpp"

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
	pollfd fd;
	fd.fd = _socket;
	fd.events = POLLIN;
	_fds.push_back(fd);

}

void Server::startServer(){
	try {
		setSocket();
	}
	catch(std::exception& e) {
		std::cerr << e.what() << '\n';
		return ;
	}

	int clientSocket;

	sockaddr_in clientAddress;
	socklen_t clientAddressSize = sizeof(clientAddress);
	clientSocket = accept(_socket, (sockaddr *)&clientAddress, &clientAddressSize);
	if (clientSocket ==  -1)
		throw(ClientAcceptFail());
	else
		std::cout << "Client connected!" <<std::endl;
	
	pollfd fd;
	fd.fd = clientSocket;
	fd.events = POLLIN;
	_fds.push_back(fd);

	// std::string hello = "Hallo von Server!";
	// i = send(clientSocket, hello.c_str(), hello.size(), 0);

	while (1) {
		int pollVal = poll(_fds.data(), _fds.size(), -1);
		if (pollVal == -1)
			throw(PollFail());
		// std::vector<pollfd>::iterator iter = _fds.begin();
		for (size_t i = 0; i < _fds.size(); i++) {
			if (_fds[i].revents & (POLLHUP | POLL_ERR | POLLNVAL))
			{
				_fds.erase(_fds.begin()+i);
				_fds.shrink_to_fit();
			}	
			if (_fds[i].revents & POLLIN) {
				if (_fds[i].fd == _socket) {
					std::cout << "!!!!Socket => "<< i << std::endl;
					sockaddr_in clientAddress;
					socklen_t clientAddressSize = sizeof(clientAddress);
					std::cout << "before accept!" << std::endl;
					clientSocket = accept(_socket, (sockaddr *)&clientAddress, &clientAddressSize);
					std::cout << "after accept!" << std::endl;
					if (clientSocket ==  -1)
						throw(ClientAcceptFail());
					else
						std::cout << "Client connected!" << std::endl;
					pollfd new_fd;
					new_fd.fd = clientSocket;
					new_fd.events = POLLIN;
					_fds.push_back(new_fd);
					_fds[i].revents = 0;
					_fds[i].events = POLLIN;
				}
				else
				{
					//std::cout << "Socket => "<< i << std::endl;
					char buffer[1024] = {0};
					int valread;
					valread = read(_fds[i].fd, buffer, 1024);
					if (buffer[0] != 0)
						std::cout << _fds.size() << "> " << buffer << std::endl;
					buffer[0] = 0;
					const std::string msg = ":Server opening Hallo, was geht\r\n";
					send(_fds[i].fd, msg.c_str(), msg.size(), 0);
					_fds[i].revents = 0;
					_fds[i].events = POLLIN;
				}
			}
			// iter++;
			// std:
		}
	}
	close(_socket);
}