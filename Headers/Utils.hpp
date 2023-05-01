#pragma once
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
#include <limits>

#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "TranslateBNF.hpp"

// used for some returns
typedef enum e_rpl{
	rpl_no_rpl = -1,
	rpl_default = 0,
	rpl_RPL_TOPIC = 332,
	rpl_ERR_INVITEONLYCHAN = 473,
	rpl_ERR_BADCHANNELKEY = 475,
}t_rpl;



bool	isChannel(std::vector<Channel>& channels, std::string channelName);
bool	isUser(std::vector<User> users, std::string nickname);

// int		getChannel(std::vector<Channel> channels, std::string channelName);
int		getUser(std::vector<User> users, std::string nickname);

//  NOTE USE THE  NICKNAME AFER THE NUMBER
// PASS
#define ERR_ALREADYREGISTRED "462 :You may not reregister\r\n"
#define ERR_PASSWDMISMATCH "462 :Password incorrect\r\n"

// MODES
#define ERR_NEEDMOREPARAMS(nickname, command) "461 " + nickname + " " + command + " :Not enough parameters\r\n"						// "<command> :Not enough parameters"
#define ERR_CHANOPRIVSNEEDED(nickname, channel) "482 " + nickname + " " + channel + " :You're not channel operator\r\n"					// "<channel> :You're not channel operator"
#define ERR_NOTONCHANNEL(nickname, channel)  "442 " + nickname + " " + channel + " :You're not on that channel\r\n"						// "<channel> :You're not on that channel"
#define RPL_BANLIST(nickname, channel) "367 " + nickname + " " + command + " " + banid "\r\n"												// "<channel> <banid>"
#define ERR_UNKNOWNMODE(nickname, char1) "472 " + nickname + " " + char1 + " :is unknown mode char to me\r\n"								// "<char> :is unknown mode char to me"
#define ERR_USERSDONTMATCH(nickname) "502 " + nickname + " :Cant change mode for other users\r\n"										// ":Cant change mode for other users"
#define ERR_UMODEUNKNOWNFLAG(nickname) "501 " + nickname + ":Unknown MODE flag\r\n"													// ":Unknown MODE flag"
#define RPL_CHANNELMODEIS(nickname, channel, mode, modeParams) "324 " + nickname + " " + channel + " " + mode + " " + modeParams + "\r\n"			// "<channel> <mode> <mode params>"									// "<nickname> :No such nick/channel"
#define ERR_KEYSET(nickname, channel) "467 " + nickname + " " + channel + " :Channel key already set\r\n"									// "<channel> :Channel key already set"
#define RPL_ENDOFBANLIST(nickname, channel) "368 " + nickname + " " + channel + " :End of channel ban list\r\n";							// "<channel> :End of channel ban list"
#define ERR_NOSUCHCHANNEL(nickname, channelWithPrefix) "403 " + nickname + " " + channelWithPrefix + " :No such channel\r\n"				// "<channel name> :No such channel"
#define RPL_UMODEIS(nickname, mode) "221 " + nickname + " " + mode + "\r\n"																// "<user mode string>"

// JOIN
#define ERR_INVITEONLYCHAN(nickname, channel) "473 " + nickname + " " + channel + " :Cannot join channel (+i)\r\n"						// "<channel> :Cannot join channel (+i)"
#define ERR_CHANNELISFULL(nickname, channel) "471 " + nickname + " " + channel + " :Cannot join channel (+l)\r\n"							// "<channel> :Cannot join channel (+l)"
#define RPL_TOPIC(nickname, channel, topic) "332 " + nickname + " " + channel + " :" + topic + "\r\n" 											// "<channel> :<topic>"
#define ERR_BANNEDFROMCHAN(nickname, channel) "474 " + nickname + " " + channel + " :Cannot join channel (+b)\r\n"						// "<channel> :Cannot join channel (+b)"
#define ERR_BADCHANNELKEY(nickname, channel) "475 " + nickname  + " " + channel + " :Cannot join channel (+k)\r\n"							// "<channel> :Cannot join channel (+k)"

// PRIVATE MESSAGE
#define ERR_NORECIPIENT(nickname, command) "411 " + nickname + " :No recipient given (" + command + ")\r\n" 								// ":No recipient given (<command>)"
#define ERR_CANNOTSENDTOCHAN(nickname, channelWithPrefix) "404 " + nickname + " " channelWithPrefix " :Cannot send to channel\r\n"	// "<channel name> :Cannot send to channel"
#define ERR_WILDTOPLEVEL(nickname, mask) "414 " + nickname + " " + mask + " :Wildcard in toplevel domain\r\n"								// "<mask> :Wildcard in toplevel domain"
#define RPL_AWAY(nickname, nickReceiving, message) "414 " + nickname + " " + nickReceiving + " :$ " message + "\r\n"						// "<nick> :<away message>"
#define ERR_NOTEXTTOSEND(nickname) "412 " + nickname + " :No text to send\r\n"														// ":No text to send"
#define ERR_NOTOPLEVEL(nickname, mask) "413 " + nickname + " " + mask + " :No toplevel domain specified\r\n" 								// "<mask> :No toplevel domain specified"
#define ERR_TOOMANYTARGETS(nickname, target) "407 " + nickname + " " + target + " :Duplicate recipients. No message delivered\r\n"			// "<target> :Duplicate recipients. No message delivered"

// TOPIC
#define RPL_NOTOPIC(nickname, channel) "331 " + nickname + " " + channel + " :No topic is set\r\n"										// "<channel> :No topic is set"
#define ERR_CHANOPRIVSNEEDED(nickname, channel) "482 " + nickname + " " + channel + " :You're not channel operator\r\n"					// "<channel> :You're not channel operator"

// INVITE
#define RPL_INVITING(nickname, channel) "341 " + channel + " " + nickname + "\r\n"														// "<channel> <nick>"
#define ERR_NOSUCHNICK(nickname) "401 " + nickname + " :No such nick/channel\r\n"														// "<nickname> :No such nick/channel"
#define ERR_USERONCHANNEL(nickname, channel) "443 " + nickname + " " + channel + " :is already on channel\r\n"							// "<user> <channel> :is already on channel"

//   NICK
#define ERR_NONICKNAMEGIVEN() "431 :No nickname given\r\n"
#define ERR_NICKNAMEINUSE(nickname) "433" + nickname + ":No nickname given\r\n"
#define ERR_NICKCOLLISION(nickname) "436" + nickname + ":Nickname collision KILL\r\n"
#define ERR_ERRONEUSNICKNAME(nickname) "432" + nickname + ":Erroneus nickname\r\n"



// Not Sorted yet
#define USER_LIST(nickname,channel,user1,user2) "353 " + nickname +" " + channel + " :" + user1 + " " + user2 + "test_fail" + "\r\n"
#define RPL_ENDOFNAMES(nickname, channel) "366 " + nickname + " " + channel + "\r\n"
#define RPL_JOIN(nickname, channel) ":" + nickname + " JOIN :" + channel + "\r\n"

#endif
