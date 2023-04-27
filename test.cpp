#include <iostream>
#include <string>

#define TEST(a, b, c) "this " a b c " end"
#define RPL_TOPIC(nickname, channel, topic) "332 " nickname " " channel " :" topic "\13\10"											// "<channel> :<topic>"

void print(std::string a)
{
	using namespace std;
	cout << a << endl;
}

int main()
{
	using namespace std;
	print(RPL_TOPIC("sucks", "for", "real"));
	return (0);
}