
#ifndef CLASS_BNF_Translate
# define CLASS_BNF_Translate

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
class BNF_Translate
{
			s_prefix				prefix;//optional
			s_command				command;
			std::vector<s_param>	params;
		//'/10''/13' aka crlf
		void					deconstruct_params( std::string& ,int& );
		void					deconstruct_msg( std::string );
		void					deconstruct_prefix( std::string& ,int& );
		void					deconstruct_command( std::string& ,int& );
	public:
		std::string				get_full_msg( void );
		void					set_prefix_servername(std::string servername);
		void					set_prefix_nick(std::string nick);
		void					set_prefix_user(std::string user);
		void					set_prefix_host(std::string host);
		void					setter_command( std::string command );
		void					setter_num( std::string num );
		void					setter_params( std::vector<s_param> params );
		std::vector<s_param>	getter_params( void );
		std::string				getter_num( void );
		std::string				getter_command( void );
		std::string				get_prefix_servername( void );
		std::string				get_prefix_nick( void );
		std::string				get_prefix_user( void );
		std::string				get_prefix_host( void );
		std::string				get_full_string();
		BNF_Translate();
		BNF_Translate(std::string);
		BNF_Translate(const BNF_Translate &a);
		~BNF_Translate();
		BNF_Translate &operator= (const BNF_Translate& a);
};

#endif