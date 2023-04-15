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


bool	isChannel(std::vector<Channel> channels, std::string channelName);
bool	isUser(std::vector<User> users, std::string nickname);

int		getChannel(std::vector<Channel> channels, std::string channelName);
int		getUser(std::vector<User> users, std::string nickname);

//  NOTE USE THE  NICKNAME AFER THE NUMBER
// MODES
#define ERR_NEEDMOREPARAMS(nickname, command) "462 " nickname " " command " :Not enough parameters\13\10"						// "<command> :Not enough parameters"
#define ERR_CHANOPRIVSNEEDED(nickname, channel) "482 " nickname " " channel " :You're not channel operator\13\10" ;					// "<channel> :You're not channel operator"
#define ERR_NOTONCHANNEL(nickname, channel)  "442 " nickname " " command " :You're not on that channel\13\10";						// "<channel> :You're not on that channel"
#define RPL_BANLIST(nickname, channel) "367 " nickname " " command " " banid "\13\10";												// "<channel> <banid>"
#define ERR_UNKNOWNMODE(nickname, char1) "472 " nickname " " char1 " :is unknown mode char to me\13\10";								// "<char> :is unknown mode char to me"
#define ERR_USERSDONTMATCH(nickname) "502 " nickname " :Cant change mode for other users\13\10" ;										// ":Cant change mode for other users"
#define ERR_UMODEUNKNOWNFLAG(nickname) "501 " nickname " :Unknown MODE flag\13\10" ;													// ":Unknown MODE flag"
#define RPL_CHANNELMODEIS(nickname, channel, mode, modeParams) "324 " nickname " " channel " " mode " " modeParams "\13\10" ;			// "<channel> <mode> <mode params>"									// "<nickname> :No such nick/channel"
#define ERR_KEYSET(nickname, channel) "467 " nickname " " channel " :Channel key already set\13\10" ;									// "<channel> :Channel key already set"
#define RPL_ENDOFBANLIST(nickname, channel) "368 " nickname " " channel " :End of channel ban list\13\10";							// "<channel> :End of channel ban list"
#define ERR_NOSUCHCHANNEL(nickname, channelWithPrefix) "403 " nickname " " channelWithPrefix " :No such channel\13\10" ;				// "<channel name> :No such channel"
#define RPL_UMODEIS(nickname, mode) "221 " nickname " " mode "\13\10" ;																// "<user mode string>"

// JOIN
#define ERR_INVITEONLYCHAN(nickname, channel) "473 " nickname " " channel " :Cannot join channel (+i)\13\10" ;						// "<channel> :Cannot join channel (+i)"
#define ERR_CHANNELISFULL(nickname, channel) "471 " nickname " " channel " :Cannot join channel (+l)\13\10" ;							// "<channel> :Cannot join channel (+l)"
#define RPL_TOPIC(nickname, channel, topic) "332 " nickname " " channel " :" topic "\13\10" ;											// "<channel> :<topic>"
#define ERR_BANNEDFROMCHAN(nickname, channel) "474 " nickname " " channel " :Cannot join channel (+b)\13\10" ;						// "<channel> :Cannot join channel (+b)"
#define ERR_BADCHANNELKEY(nickname, channel) "475 " nickname " " channel " :Cannot join channel (+k)\13\10" ;							// "<channel> :Cannot join channel (+k)"

// PRIVATE MESSAGE
#define ERR_NORECIPIENT(nickname, command) "411 " nickname " :No recipient given (" command ")\13\10" ; 								// ":No recipient given (<command>)"
#define ERR_CANNOTSENDTOCHAN(nickname, channelWithPrefix) "404 " nickname " " channelWithPrefix " :Cannot send to channel\13\10" ;	// "<channel name> :Cannot send to channel"
#define ERR_WILDTOPLEVEL(nickname, mask) "414 " nickname " " mask " :Wildcard in toplevel domain\13\10" ;								// "<mask> :Wildcard in toplevel domain"
#define RPL_AWAY(nickname, nickReceiving, message) "414 " nickname " " nickReceiving " :$ " message "\13\10" ;						// "<nick> :<away message>"
#define ERR_NOTEXTTOSEND(nickname) "412 " nickname " :No text to send\13\10" ;														// ":No text to send"
#define ERR_NOTOPLEVEL(nickname, mask) "413 " nickname " " mask " :No toplevel domain specified\13\10"; 								// "<mask> :No toplevel domain specified"
#define ERR_TOOMANYTARGETS(nickname, target) "407 " nickname " " target " :Duplicate recipients. No message delivered\13\10";			// "<target> :Duplicate recipients. No message delivered"

// TOPIC
#define RPL_NOTOPIC(nickname, channel) "331 " nickname " " channel " :No topic is set\13\10" ;										// "<channel> :No topic is set"
#define ERR_CHANOPRIVSNEEDED(nickname, channel) "482 " nickname " " channel " :You're not channel operator\13\10" ;					// "<channel> :You're not channel operator"

// INVITE
#define RPL_INVITING(nickname, channel) "341 " channel " " nickname "\13\10" ;														// "<channel> <nick>"
#define ERR_NOSUCHNICK(nickname) "401 " nickname " :No such nick/channel\13\10" ;														// "<nickname> :No such nick/channel"
#define ERR_USERONCHANNEL(nickname, channel) "443 " nickname " " channel " :is already on channel\13\10" ;							// "<user> <channel> :is already on channel"

#endif
