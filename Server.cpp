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
}

void Server::startServer(){
	try {
		setSocket();
	}
	catch(std::exception& e) {
		std::cerr << e.what() << '\n';
		return ;
	}

	// std::vector<pollfd>::iterator iter = _fds.begin();
	int clientSocket;

	sockaddr_in clientAddress;
	socklen_t clientAddressSize = sizeof(clientAddress);
	clientSocket = accept(_socket, (sockaddr *)&clientAddress, &clientAddressSize);
	if (clientSocket ==  -1)
		throw(ClientAcceptFail());
	else
		std::cout << "Client connected!" <<std::endl;
	
	std::string hello = "Hallo von Server!";
	int i; 
	i = send(clientSocket, hello.c_str(), hello.size(), 0);

	while (1) {
		// int pollVal = poll(_fds.data(), _fds.size(), -1);
		// if (pollVal == -1)
		// 	throw(PollFail());
		// for (int i = 0; i < _fds.size(); i++) {
		// 	if (_fds[i].revents & POLLIN) {
		// 		if (_fds.data()->fd == _socket) {
		// 			// sockaddr_in clientAddress;
		// 			// socklen_t clientAddressSize = sizeof(clientAddress);
		// 			// clientSocket = accept(_socket, (sockaddr *)&clientAddress, &clientAddressSize);
		// 			// if (clientSocket ==  -1)
		// 			// 	throw(ClientAcceptFail());
		// 			// else
		// 			// 	std::cout << "Client connected!" << std::endl;
		// 			// pollfd new_fd;
		// 			// new_fd.fd = clientSocket;
		// 			// new_fd.events = POLLIN;
		// 		}
		// 	}
		// 	iter++;
			char buffer[1024] = {0};
			int valread;
			valread = read(clientSocket, buffer, 1024);
			std::cout << buffer << std::endl;
			i = send(clientSocket, hello.c_str(), hello.size(), 0);
		}
	close(_socket);
}