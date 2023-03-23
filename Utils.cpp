#ifndef UTILS_HPP
# define UTILS_HPP

#include <iostream>
#include <vector>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>

// MODES
#define ERR_NEEDMOREPARAMS "462 $ :Not enough parameters" ;
#define ERR_CHANOPRIVSNEEDED "482 $ :You're not channel operator" ;
#define ERR_NOTONCHANNEL  "442 $ :You're not on that channel";
#define RPL_BANLIST "367 $ $";
#define ERR_UNKNOWNMODE "472 $ :is unknown mode char to me";
#define ERR_USERSDONTMATCH "502 :Cant change mode for other users" ;
#define ERR_UMODEUNKNOWNFLAG "501 :Unknown MODE flag" ;
#define RPL_CHANNELMODEIS "324 $ $ $" ;
#define ERR_NOSUCHNICK "401 $ :No such nick/channel" ;
#define ERR_KEYSET "467 $ :Channel key already set" ;
#define RPL_ENDOFBANLIST "368 $ :End of channel ban list";
#define ERR_NOSUCHCHANNEL "403 $ :No such channel" ;
#define RPL_UMODEIS "221 $" ;

// JOIN
#define ERR_INVITEONLYCHAN "473 $ :Cannot join channel (+i)" ;
#define ERR_CHANNELISFULL "471 $ :Cannot join channel (+l)" ;
#define RPL_TOPIC "332 $ :$" ;
#define ERR_BANNEDFROMCHAN "474 $ :Cannot join channel (+b)" ;
#define ERR_BADCHANNELKEY "475 $ :Cannot join channel (+k)" ;


// PRIVATE MESSAGE
#define ERR_NORECIPIENT "411 :No recipient given ($)" ; // COMMAND
#define ERR_CANNOTSENDTOCHAN "404 :Cannot send to channel" ; // CHANNEL NAME
#define ERR_WILDTOPLEVEL "414 $ :Wildcard in toplevel domain" ; // MASK
#define RPL_AWAY "414 $ :$ message>" ; // <nick> :<away message>
#define ERR_NOTEXTTOSEND "412:No text to send" ;
#define ERR_NOTOPLEVEL "413 $ :No toplevel domain specified"; // <mask> :No toplevel domain specified
#define ERR_TOOMANYTARGETS "407 $ :Duplicate recipients. No message delivered"; // "<target> :Duplicate recipients. No message delivered


// TOPIC
#define RPL_NOTOPIC "331 $ :No topic is set" ; // <channel> :No topic is set
#define ERR_CHANOPRIVSNEEDED "482 $ :You're not channel operator" ; // "<channel> :You're not channel operator"

// INVITE
#define RPL_INVITING "341 $ $" ; // <channel> <nick>
#define ERR_NOSUCHNICK "401 $ :No such nick/channel" ; // <nickname> :No such nick/channel
#define ERR_USERONCHANNEL "443 $ $ :is already on channel" ; //<user> <channel> :is already on channel

#endif