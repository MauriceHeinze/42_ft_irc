#include "./Headers/Utils.hpp"
// #include "./Headers/Server.hpp"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cout << "Start server with: ./ircserv <port> <password>" << std::endl;
        return (1);
    }
    else
    {
        // start server
		Server server(argv);
		try{
			server.startServer();
		}
		catch (std::exception &e) {
			std::cerr << e.what() << '\n';
		}
        // std::cout << "Port:     " << argv[1] << std::endl;
        // std::cout << "Password: " << argv[2] << std::endl;
    }

    return (0);
}