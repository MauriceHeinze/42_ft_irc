
#include	"./Headers/Connection.hpp"

std::string Connection::get_password( void )
{
	return(this->_password);
}

void Connection::set_password(std::string new_passwort= "")
{
	this->_password = new_passwort;
}

Connection::Connection()
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