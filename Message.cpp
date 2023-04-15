#include "./Headers/TranslateBNF.hpp"
#include <iostream>
#include <string>

int main()
{
	std::string buffer;
	while(1)
	{
		buffer.resize(100);
		std::cout << "Type Message" << std::endl;
		std::cin.getline((char *)buffer.c_str(),100);
		TranslateBNF	msg(buffer);
		buffer.clear();
	}
}