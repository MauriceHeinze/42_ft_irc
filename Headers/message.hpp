
#ifndef CLASS_message
# define CLASS_message

#include	<string>
#include	<vector>

//<prefix>   ::= <servername> | <nick> [ '!' <user> ] [ '@' <host> ]
struct s_prefix{
	//':'
	bool			nick;// bool for nick or servername
	std::string		nick_or_servername;// will only use nick for now
	// '!'
	std::string		user;//optional
	// '@'
	std::string		host;//optional
	//' ' //space
};

//<command>  ::= <letter> { <letter> } | <number> <number> <number>
struct s_command{
	std::string			command;
	std::string			num;// 1 2 3
};

//<params>   ::= <SPACE> [ ':' <trailing> | <middle> <params> ]
struct s_param{
	//' ' //space
	//':'
	bool		trailing;
	std::string trailing_or_middle;
};

//<message>  ::= [':' <prefix> <SPACE> ] <command> <params> <crlf>
class message
{
	private:
		s_prefix					prefix;//optional
		s_command					command;
		std::vector<s_param>		params;
		//'/10''/13' aka crlf


		void						deconstruct_msg(std::string);
		void						deconstruct_prefix(std::string& ,int&);
		void						deconstruct_command(std::string& ,int&);
	public:
		message();
		message(std::string);
		message(const message &a);
		~message();
		message &operator= (const message& a);
};

#endif
