#include "../inc/returncode.hpp"

std::string RPL_NOTICE(std::string source, std::string target, std::string message) {
	return ":" + source + " NOTICE " + target + " :" + message;
}

std::string RPL_PONG(std::string source, std::string message) {
	return ":" + source + " PONG :" + message;
}

std::string RPL_JOIN(std::string source, std::string channel) {
	return ":" + source + " JOIN :" + channel;
}

std::string RPL_PART(std::string source, std::string channel, std::string msg) {
	return ":" + source + " PART " + channel + " :" + msg;
}

std::string RPL_WELCOME(std::string nick, std::string server, std::string user, std::string hostname){
	return ":" + server + " 001 " + nick + " :Welcome to the " + server + " network, " + nick + "[" + user + "@" + hostname + "]";
}

std::string RPL_YOURHOST(std::string nick, std::string server){
	return ":" + server + " 002 " + nick + " :Your host is " + server + ", running version 1.2.3";
}

std::string RPL_CREATED(std::string nick, std::string server , std::string date){
	return ":" + server + " 003 " + nick + " :This server was created " + date;
}

std::string RPL_MYINFO(std::string nick, std::string server){
	return ":" + server + " 004 " + nick + " " + server + " irssi 1.2.3 (20210409 0011)" ;
}

std::string RPL_UMODEIS(std::string nick, std::string mode) {
	return "221 " + nick + " :" + mode;
}

std::string RPL_ENDOFWHO(std::string nick, std::string param) {
	return "315 " + nick +" "+param+" :End of /WHO list.";
}

std::string RPL_LISTSTART(std::string nick) {
	return "321 " + nick + " Channel :Users Name";
}

std::string RPL_LIST(std::string nick, std::string channel, std::string topic) {
	return "322 " + nick + " " + channel + " :" + topic;
}

std::string RPL_LISTEND(std::string nick) {
	return "323 " + nick + " :End of /LIST";
}

std::string RPL_CHANNELMODEIS(std::string nick, std::string channel, std::string mode, std::string modeparam) {
	return "324 " + nick + " MODE " + channel + " " + mode + " " + modeparam;
}

std::string RPL_NOTOPIC(std::string nick, std::string channel) {
	return "331 " + nick + ": " + channel + ":No topic is set";
}

std::string RPL_TOPIC(std::string nick, std::string channel, std::string topic) {
	return "332 " + nick + " " + channel + " :" + topic;
}

std::string	RPL_INVITING(std::string nick, std::string target, std::string channel) {
	return "341 " + nick + ": " + target + ": " + channel;
}

std::string RPL_WHOREPLY(std::string nick, std::string channel, std::string user, std::string host, std::string server, std::string nick2, std::string flags, std::string hopcount, std::string realname) {
	return "352 " + nick + " " + channel + " " + user + " " + host + " " + server + " " + nick2 + " " + flags + " :" + hopcount + " " + realname;
}

std::string RPL_NAMREPLY(std::string prefix, std::string nick, std::string channel, std::string user) {
	return ":" + prefix + " 353 " + nick + " = " + channel + " :" + user;
}

std::string RPL_ENDOFNAMES(std::string prefix, std::string nick, std::string channel) {
	return ":" + prefix + " 366 " + nick + " " + channel + " :End of /NAMES list";
}


std::string RPL_MOTDSTART() {
	return "375 ";
}

std::string RPL_ENDOFMOTD() {
	return "376 ";
}

std::string RPL_YOUROPER(std::string nick, std::string arg1, std::string arg2) {
	(void)arg1;
	(void)arg2;
	return "381 " + nick + " :You are now an IRC operator";
	// return "381 " + nick + ":Attempt to register as an operator using a username of '" + arg1 + "' and '" + arg2 + "' as the password.";
}

std::string RPL_NOTONCHANNEL() {
	return "442 ";
}

/* -------------------------------------------------------------------------- */
/* ERRORS                                                                     */
/* -------------------------------------------------------------------------- */

std::string ERR_CANNOTSENDTOCHAN(std::string source, std::string channel) {
	return "404 " + source + " " + channel + " :Cannot send to channel";
}


std::string ERR_NOSUCHNICK(std::string nick, std::string arg) {
	return "401 " + nick + ": " + arg + ":No such nick";
}

std::string ERR_NOSUCHSERVER(std::string nick, std::string arg) {
	return "402 " + nick + ": " + arg + ":No such server";
}

std::string ERR_NOSUCHCHANNEL(std::string nick, std::string arg) {
	return "403 " + nick + ": " + arg + ":No such channel";
}

std::string	ERR_TOOMANYCHANNELS(std::string nick) {
	return "405 " + nick + ":You have joined too many channels";
}

std::string ERR_NOORIGIN(const std::string nick) {
	return "409 " + nick + ":No origin specified";
}

std::string ERR_NONICKNAMEGIVEN(std::string nick) {
	return "431 " + (nick.empty() ?  "*" : nick) + ":No nickname given";
}

std::string ERR_NICKNAMEINUSE(std::string nick) {
	return "433 " + (nick.empty() ?  "*" : nick) + ":Nickname is already in use";
}

std::string ERR_ERRONEUSNICKNAME (std::string nick, std::string nick2)
{
	return "462 "+ (nick.empty() ?  "*" : nick) + " " + nick2 + " :Erroneus nickname";
}

std::string ERR_NICKCOLLISION() {
	return "436 ";
}

std::string ERR_USERNOTINCHANNEL() {
	return "441 ";
}

std::string ERR_NOTONCHANNEL(std::string nick, std::string channel) {
	return "442 " + nick + ": " + channel + ":You're not on that channel";
}

std::string ERR_USERONCHANNEL(std::string nick, std::string channel) {
	return "443 " + nick + ": " + channel + ":is already on channel";
}

std::string ERR_NOTREGISTERED(std::string nick) {
	return "451 " + (nick.empty() ?  "*" : nick) + " " + ":You have not registered";
}

std::string ERR_NEEDMOREPARAMS(std::string nick, std::string arg) {
	return "461 " + (nick.empty() ?  "*" : nick) + " " + arg + " :Not enough parameters";
}

std::string ERR_ALREADYREGISTRED(std::string nick) {
	return "462 " + nick + ":You may not reregister";
}

std::string ERR_ERRONEUSNICKNAME() {
	return "462 ";
}

std::string ERR_PASSWDMISMATCH(std::string nick) {
	return "464 " + (nick.empty() ?  "*" : nick) + " :Password incorrect";
}

std::string ERR_CHANNELISFULL(std::string nick, std::string channel) {
	return "471 " + nick + ": " + channel + " :Cannot join channel (+l)";
}

std::string ERR_UNKNOWNMODE() {
	return "472 ";
}

std::string ERR_INVITEONLYCHAN(std::string nick, std::string channel) {
	return "473 " + nick + " " + channel + " :Cannot join channel (+i)";
}

std::string ERR_BANNEDFROMCHAN(std::string nick) {
	return "474 " + nick + "<canal> :Cannot join channel (+b)";
}

std::string ERR_BADCHANNELKEY(std::string nick, std::string channel) {
	return "475 " + nick + " " + channel + " :Cannot join channel (+k)";
}

std::string ERR_NOPRIVILEGES(std::string nick) {
	return "481 " + nick + ":Permission Denied- You're not an IRC operator";
}

std::string ERR_CHANOPRIVSNEEDED(std::string nick, std::string channel) {
	return "482 " + nick + ": " + channel + ":You're not channel operator";
}

std::string ERR_UMODEUNKNOWNFLAG(std::string nick, std::string flag) {
	return "501 " + nick + " :" + flag + " Unkown MODE flag";;
}

std::string ERR_USERSDONTMATCH(std::string nick) {
	return "502 " + nick + " :Cannot change mode for other users";
}