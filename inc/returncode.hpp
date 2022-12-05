#ifndef RETURNCODE_HPP
# define RETURNCODE_HPP

# include <string>

// RPL
std::string RPL_NOTICE(std::string source, std::string target, std::string message);
std::string RPL_PONG(std::string source, std::string message);
std::string RPL_JOIN(std::string source, std::string channel);
std::string RPL_PART(std::string source, std::string channel, std::string msg);
std::string RPL_WELCOME(std::string nick, std::string server, std::string user, std::string hostname);
std::string RPL_YOURHOST(std::string nick, std::string server);
std::string RPL_CREATED(std::string nick, std::string server , std::string date);
std::string RPL_MYINFO(std::string nick, std::string server);
std::string RPL_LISTSTART(std::string nick);
std::string RPL_LIST(std::string nick, std::string channel, std::string topic);
std::string RPL_LISTEND(std::string nick);
std::string RPL_NOTOPIC(std::string nick, std::string channel);
std::string RPL_TOPIC(std::string nick, std::string channel, std::string topic);
std::string	RPL_INVITING(std::string nick, std::string target, std::string channel);
std::string RPL_NAMREPLY(std::string prefix, std::string nick, std::string channel, std::string user);
std::string RPL_ENDOFNAMES(std::string prefix, std::string nick, std::string channel);
std::string RPL_MOTDSTART();
std::string RPL_ENDOFMOTD();
std::string RPL_YOUROPER(std::string nick, std::string arg1, std::string arg2);
std::string RPL_NOTONCHANNEL();
std::string RPL_ENDOFWHO(std::string nick);
std::string RPL_WHOREPLY(std::string nick, std::string channel, std::string user, std::string host, std::string server, std::string nick2, std::string flags, std::string hopcount, std::string realname);
std::string RPL_CHANNELMODEIS(std::string nick, std::string channel, std::string mode);
std::string RPL_UMODEIS(std::string nick, std::string mode);
// ERR
std::string ERR_NOORIGIN(const std::string nick);
std::string ERR_NOSUCHNICK(std::string nick, std::string arg);
std::string ERR_NOTREGISTERED(std::string nick);
std::string ERR_NOSUCHCHANNEL(std::string nick, std::string arg);
std::string ERR_CANNOTSENDTOCHAN(std::string source, std::string channel);
std::string	ERR_TOOMANYCHANNELS(std::string nick);
std::string ERR_NONICKNAMEGIVEN(std::string nick);
std::string ERR_NICKNAMEINUSE(std::string nick);
std::string ERR_ERRONEUSNICKNAME (std::string nick, std::string nick2);
std::string ERR_NICKCOLLISION();
std::string ERR_USERNOTINCHANNEL();
std::string ERR_NOTONCHANNEL(std::string nick, std::string channel);
std::string ERR_USERONCHANNEL(std::string nick, std::string channel);
std::string ERR_NEEDMOREPARAMS(std::string nick, std::string arg);
std::string ERR_ALREADYREGISTRED(std::string nick);
std::string ERR_ERRONEUSNICKNAME();
std::string ERR_PASSWDMISMATCH(std::string nick);
std::string ERR_CHANNELISFULL(std::string nick, std::string channel);
std::string ERR_UNKNOWNMODE();
std::string ERR_INVITEONLYCHAN(std::string nick, std::string channel);
std::string ERR_BANNEDFROMCHAN(std::string nick);
std::string ERR_BADCHANNELKEY(std::string nick, std::string channel);
std::string ERR_NOPRIVILEGES(std::string nick);
std::string ERR_CHANOPRIVSNEEDED(std::string nick, std::string channel);
std::string ERR_UMODEUNKNOWNFLAG(std::string nick, std::string flag);
std::string ERR_USERSDONTMATCH(std::string nick);


#endif