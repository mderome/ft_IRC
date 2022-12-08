#include "../inc/user.hpp"
#include "../inc/server.hpp"
#include "../inc/utils.hpp"

// CONNECTION  COMMANDES
void	Server::_indexingCmd(){
	_indexCmd.insert(std::pair<std::string, func>("USER", &Server::_userCmd)); 
	_indexCmd.insert(std::pair<std::string, func>("PASS", &Server::_passCmd));
	_indexCmd.insert(std::pair<std::string, func>("NICK", &Server::_nickCmd));
	_indexCmd.insert(std::pair<std::string, func>("CAP", &Server::_caplsCmd));
	_indexCmd.insert(std::pair<std::string, func>("PING", &Server::_pingCmd));
	_indexCmd.insert(std::pair<std::string, func>("QUIT", &Server::_quitCmd));
	_indexCmd.insert(std::pair<std::string, func>("JOIN", &Server::_joinCmd));
	// _indexCmd.insert(std::pair<std::string, func>("PART", &Server::_partCmd));
	_indexCmd.insert(std::pair<std::string, func>("PRIVMSG", &Server::_privmsgCmd));
	_indexCmd.insert(std::pair<std::string, func>("LIST", &Server::_listCmd));
	// _indexCmd.insert(std::pair<std::string, func>("MODE", &Server::_modeCmd));
	// _indexCmd.insert(std::pair<std::string, func>("WHOIS", &Server::_whoisCmd));
	// _indexCmd.insert(std::pair<std::string, func>("KICK", &Server::_kickCmd));
	// _indexCmd.insert(std::pair<std::string, func>("MOTD", &Server::_motdCmd));
}

void	Server::chooseCmd(User *user)
{
	std::string	msg = user->getMessage();
	std::string	cmd;
	std::string	buf;

	while (msg.length())
	{
		if (msg.find("\r\n") != std::string::npos)
		{
			if (msg.find(' ') != std::string::npos)
				cmd = msg.substr(0, msg.find(' '));
			else
				cmd = msg.substr(0, msg.find("\r\n"));
			if (cmd.length() == msg.find("\r\n"))
				buf.clear();
			else
				buf = msg.substr(cmd.length() + 1, msg.find("\r\n") - cmd.length() - 1);
			try
			{
				if (buf.find_first_not_of(' ') != std::string::npos)
					buf = buf.substr(buf.find_first_not_of(' '));
				else
					buf.clear();
				if (!buf.empty())
					buf = buf.substr(0, buf.find_last_not_of(' ') + 1);
				std::cout << CYAN "Command: <" WHITE << cmd << CYAN ">" << std::endl;
				std::cout << CYAN "Buffer: <" WHITE << buf << CYAN ">" << std::endl;
				if (cmd != "CAP" && cmd != "PASS" && !user->getPassword())
				{
					closeConnection(user);
					break;
				}
				
				for (std::map<std::string, func>::iterator it = this->_indexCmd.begin(); it != this->_indexCmd.end(); it++)
				{
					if (it->first == cmd)
					{
						(this->*(it->second))(user, buf); // call function from map, giving user and buf as parameters
						break;
					}
				}
				msg.erase(0, msg.find("\r\n") + 2);
			}
			catch (const std::out_of_range &e)
			{
				//std::cout << "fail: " << e.what() << std::endl;
				msg.erase(0, msg.find("\r\n") + 2);
				user->sendReply("421 Unknown command");
			}
		}
		else
		{
			msg.clear();
			user->sendReply("Message error"); //Changer message erreur
		}
	}
}

void	Server::_caplsCmd(User *user, std::string param)
{
	std::cout << "CAP LS" << std::endl;
	if (param != "LS")
		return (user->sendReply("CAP LS command"));
}

void	Server::_userCmd(User *user, std::string param)
{
	std::cout<< param << std::endl;
	if (user->hasBeenWelcomed())
		return (user->sendReply(ERR_ALREADYREGISTRED(user->getNickname())));
	// PArsing pour recuperer les params renvoyer  ERR_NEEDMOREPARAMS si probleme
	// check username realname et mode
	if (param.empty())
		return (user->sendReply("Error: user: empty"));
	std::cout << "param : " << param << std::endl;
	// parsing here
	std::string username = param.substr(0, param.find(' '));
	std::string mode = param.substr(param.find(' ') + 1, param.find(' ', param.find(' ') + 1) - param.find(' ') - 1);
	std::string unused = param.substr(param.find(' ', param.find(' ') + 1) + 1, param.find(' ', param.find(' ', param.find(' ') + 1) + 1) - param.find(' ', param.find(' ') + 1) - 1);
	std::string realname = param.substr(param.find(':', param.find(' ', param.find(' ') + 1) + 1) + 1);
	user->setUsername(username);
	user->setRealname(realname);
	if (user->getNickname().size() && user->getPassword() && !user->hasBeenWelcomed())
		user->welcome();
}

void	Server::_passCmd(User *user, std::string param)
{
	if (user->hasBeenWelcomed())
		return (user->sendReply("Error: pass: already welcomed"));
	if (!param.length())
		return (user->sendReply("Error: pass: empty"));
	if (param.compare(_password))
	{
		user->setPassword(false);
		return (user->sendReply(ERR_PASSWDMISMATCH(user->getNickname())));
	}
	user->setPassword(true);
	// if (user->getNickname().length() && user->getUsername().length())
	// 	user->welcome();
}

void	Server::_nickCmd(User *user, std::string param)
{
	if (param.empty())
		return (user->sendReply(ERR_NONICKNAMEGIVEN(user->getNickname())));
	//PARSE NICKNAME POUR CHECK SI IL EST VALIDE
	// renvoie ERR_ERRONEUSNICKNAME
	if (param.find(' ') != std::string::npos)
		param = param.substr(0, param.find_first_of(' '));
	for (users_iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->second->getNickname() == param)
			return (user->sendReply(ERR_NICKCOLLISION()));
	}
	user->setNickname(param);
	// if (user->getUsername().length() && user->getPassword() && !user->hasBeenWelcomed())
	// 	user->welcome();
}

void	Server::_quitCmd(User *user, std::string param)
{
	if (param.empty())
		param = "No reason given by user";
	
	for (users_iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->second->getNickname() != user->getNickname())
			it->second->sendReply(":" + user->getNickname() + "!d@" + _hostname + " QUIT :Quit: " + param + ";\n" + user->getNickname() + " is exiting the network with the message: \"Quit: " + param + "\"");
	}
	// user->sendReply(":" + user->getNickname() + "!d@" + _hostname + " QUIT :Quit: " + param + ";\n" + user->getNickname() + " is exiting the network with the message: \"Quit: " + param + "\"");
	try
	{
		int	fd = user->getFd();
		close(fd);
		for (pollfd_iterator it = _pollfds.begin(); it != _pollfds.end(); ++it)
		{
			if (fd == it->fd)
			{
				_pollfds.erase(it);
				break;
			}
		}
		_users.erase(fd);
		delete user;
		std::cout << "Disconnecting user on socket " << fd << std::endl;
	}
	catch (const std::out_of_range &e) {}
}
	// // pas de parsing a faire
	// std::cout << user->getMessage() << std::endl;
	// std::cout << param << std::endl;

void	Server::_pingCmd(User *user, std::string param){
	if (param.empty())
		return (user->sendReply(ERR_NEEDMOREPARAMS(user->getNickname(), param)));
	if (param != _hostname && param != "IRC")
		return (user->sendReply(ERR_NOSUCHSERVER(user->getNickname(), param)));
	user->sendReply(RPL_PONG(user->getNickname(), param));
}

void	Server::_privmsgCmd(User *user, std::string param){
	if (param.empty())
		return (user->sendReply(ERR_NEEDMOREPARAMS(user->getNickname(), param)));
	std::string	target = param.substr(0, param.find(' '));
	std::string	message = param.substr(param.find(' ') + 1);
	if (message[0] != ':')
		return (user->sendReply(ERR_NORECIPIENT(user->getNickname())));
	message = message.substr(1);
	if (target[0] == '#')
	{
		std::cout << "channel" << std::endl;
		// channel
		_channel[target].sendToAllSaufALui(user->getNickname() ,":" + user->getNickname() + "@IRC PRIVMSG " + target + " :" + message + "\r\n");
	}
	else
	{
		std::cout << "user" << std::endl;
		// user
		for (std::map<int, User *>::iterator it = _users.begin(); it != _users.end(); ++it)
		{
			std::cout << "target : " << target << std::endl;
			if (it->second->getNickname() == target)
			{
				std::string buf = ":" + user->getNickname() + "@IRC PRIVMSG " + target + " :" + message + "\r\n";
				it->second->sendReply(buf);
				// send(it->first, buf.c_str(), buf.length(), 0);
				std::cout << YELLOW "sent to " << it->second->getNickname() << END << std::endl;
				return ;
			}
		}
	}
}

// void	Server::_operCmd(User *user, std::string param){
// 	// split param pour avoir name et password | Parameters: <name> <password>
// 	std::string	password;

// 	std::cout << param << std::endl;
// 	if (_hostname != user->getHostname())
// 		return ; // ERR_NOOPERHOST
// 	if (password != getPassword())
// 		return ; // ERR_PASSWDMISMATCH
// 	//RPL_YOUREOPER
// 	// avoir une fonction qui ajoute un operator a un channel
// }

 void	Server::_joinCmd(User *user, std::string param)
{
	User test = *(user);
	std::map<std::string, std::string>	joinChannel;
	std::vector<std::string>			channel;
	std::vector<std::string>			Pasword;

	if (!param.size())
		return (user->sendReply(ERR_NEEDMOREPARAMS(user->getNickname(), param)));
	channel = splitov(param, ',');
	std::vector<std::string>::iterator it = channel.begin();
	Pasword = splitov(param, ',');
	//std::vector<std::string>::iterator itpass = Pasword.begin();
	for (; it != channel.end(); it++)
	{
		std::string	content = *it;
		if (_channel.find(content) != _channel.end()){
			if (content.c_str()[0] != '#' && content.c_str()[0] != '&')
				return (user->sendReply(ERR_NOSUCHCHANNEL(user->getNickname(), content)));
			else
			{
				_channel.find(content)->second.addUser(test);
				std::cout << "this channel exist\n";
			}
		}
		else 
		{
			_channel.insert(std::pair<std::string, Channel>(content, Channel(test, content)));
			std::cout << "channel create" << std::endl;
			_channel[content].setUsersMode(test.getNickname(), std::string("o"), 1);
		}
	}
	return ; 
}

void	Server::_listCmd(User *user, std::string param)
{
	// if (!user->hasBeenWelcomed)
	// 	return ;	
	if (param.empty())
	{
		for (std::map<std::string, Channel>::iterator it = _channel.begin(); it != _channel.end(); ++it)
		{
			std::string buf = ":" + _hostname + " 322 " + user->getNickname() + " " + it->first + " " + std::to_string(it->second.getUsers().size()) + " :" + it->second.getTopic() + "\r\n";
			user->sendReply(buf);
		}
	}
	else
	{
		std::vector<std::string> channels = splitov(param, ',');
		for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
		{
			if (_channel.find(*it) != _channel.end())
			{
				std::string buf = ":" + _hostname + " 322 " + user->getNickname() + " " + *it + " " + std::to_string(_channel[*it].getUsers().size()) + " :" + _channel[*it].getTopic() + "\r\n";
				user->sendReply(buf);
			}
		}
	}	
	user->sendReply(":" + _hostname + " 323 " + user->getNickname() + " :End of /LIST\r\n");
}