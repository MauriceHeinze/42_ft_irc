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

//  NOTE USE THE  NICKNAME AFER THE NUMBER
// MODES
#define ERR_NEEDMOREPARAMS(nickname, command) "462 " nickname " " command " :Not enough parameters\13\10"					// "<command> :Not enough parameters"
#define ERR_CHANOPRIVSNEEDED(nickname, channel) "482 " nickname " " channel " :You're not channel operator" ;				// "<channel> :You're not channel operator"
#define ERR_NOTONCHANNEL(nickname, channel)  "442 " nickname " " command " :You're not on that channel";					// "<channel> :You're not on that channel"
#define RPL_BANLIST(nickname, channel) "367 " nickname " " command " " banid "";											// "<channel> <banid>"
#define ERR_UNKNOWNMODE(nickname, char1) "472 " nickname " " char1 " :is unknown mode char to me";							// "<char> :is unknown mode char to me"
#define ERR_USERSDONTMATCH(nickname) "502 " nickname " :Cant change mode for other users" ;									// ":Cant change mode for other users"
#define ERR_UMODEUNKNOWNFLAG(nickname) "501 " nickname " :Unknown MODE flag" ;												// ":Unknown MODE flag"
#define RPL_CHANNELMODEIS(nickname, channel, mode, modeParams) "324 " nickname " " channel " " mode " " modeParams "" ;		// "<channel> <mode> <mode params>"
#define ERR_NOSUCHNICK(nickname) "401 " nickname " " nickReceiving " :No such nick/channel" ;								// "<nickname> :No such nick/channel"
#define ERR_KEYSET(nickname, channel) "467 " nickname " " channel " :Channel key already set" ;								// "<channel> :Channel key already set"
#define RPL_ENDOFBANLIST(nickname, channel) "368 " nickname " " channel " :End of channel ban list";						// "<channel> :End of channel ban list"
#define ERR_NOSUCHCHANNEL(nickname, channelWithPrefix) "403 " nickname " " channelWithPrefix " :No such channel" ;					// "<channel name> :No such channel"
#define RPL_UMODEIS(nickname, mode) "221 " nickname " " mode "" ;														// "<user mode string>"

// JOIN
#define ERR_INVITEONLYCHAN(nickname, channel) "473 $ :Cannot join channel (+i)" ;					// "<channel> :Cannot join channel (+i)"
#define ERR_CHANNELISFULL(nickname, channel) "471 $ :Cannot join channel (+l)" ;					// "<channel> :Cannot join channel (+l)"
#define RPL_TOPIC(nickname, channel) "332 $ :$" ;													// "<channel> :<topic>"
#define ERR_BANNEDFROMCHAN(nickname, channel) "474 $ :Cannot join channel (+b)" ;					// "<channel> :Cannot join channel (+b)"
#define ERR_BADCHANNELKEY(nickname, channel) "475 $ :Cannot join channel (+k)" ;					// "<channel> :Cannot join channel (+k)"

// PRIVATE MESSAGE
#define ERR_NORECIPIENT(nickname, command) "411 :No recipient given ($)" ; 							// ":No recipient given (<command>)"
#define ERR_CANNOTSENDTOCHAN(nickname, channelWithPrefix) "404 :Cannot send to channel" ;			// "<channel name> :Cannot send to channel"
#define ERR_WILDTOPLEVEL(nickname, mask) "414 $ :Wildcard in toplevel domain" ;						// "<mask> :Wildcard in toplevel domain"
#define RPL_AWAY(nickname, nickReceiving, message) "414 $ :$ message>" ;							// "<nick> :<away message>"
#define ERR_NOTEXTTOSEND(nickname) "412:No text to send" ;											// ":No text to send"
#define ERR_NOTOPLEVEL(nickname, mask) "413 $ :No toplevel domain specified"; 						// "<mask> :No toplevel domain specified"
#define ERR_TOOMANYTARGETS(nickname, target) "407 X :Duplicate recipients. No message delivered";	// "<target> :Duplicate recipients. No message delivered"

// TOPIC
#define RPL_NOTOPIC(nickname, channel) "331 $ :No topic is set" ;									// "<channel> :No topic is set"
#define ERR_CHANOPRIVSNEEDED(nickname, channel) "482 $ :You're not channel operator" ;				// "<channel> :You're not channel operator"

// INVITE
#define RPL_INVITING(nickname, channel) "341 $ $" ;													// "<channel> <nick>"
#define ERR_NOSUCHNICK(nickname) "401 $ :No such nick/channel" ;									// "<nickname> :No such nick/channel"
#define ERR_USERONCHANNEL(nickname, channel) "443 $ $ :is already on channel" ;						// "<user> <channel> :is already on channel"

#endif