#include "./Headers/BNF_Translate.hpp"
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
		BNF_Translate	msg(buffer);
		buffer.clear();
	}
}