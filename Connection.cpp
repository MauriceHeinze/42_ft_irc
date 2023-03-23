
#include	"./Headers/Connection.hpp"

Connection::Connection( pollfd* poll_fd ) : _pollfd(poll_fd)
{
	this->_valid_password = false;
}

// Connection::Connection(const Connection &a)
// {
// }

Connection::~Connection()
{
}

// Connection& Connection::operator= (const Connection& a)
// {
// 	return (*(this));
// }