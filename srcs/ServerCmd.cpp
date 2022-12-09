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
	_indexCmd.insert(std::pair<std::string, func>("PRIVMSG", &Server::_privmsgCmd));
	_indexCmd.insert(std::pair<std::string, func>("MODE", &Server::_modeCmd));
	_indexCmd.insert(std::pair<std::string, func>("WHO", &Server::_whoCmd));
	_indexCmd.insert(std::pair<std::string, func>("NOTICE", &Server::_noticeCmd));
	_indexCmd.insert(std::pair<std::string, func>("LIST", &Server::_listCmd));
	// _indexCmd.insert(std::pair<std::string, func>("PART", &Server::_partCmd));
	// _indexCmd.insert(std::pair<std::string, func>("WHOIS", &Server::_whoisCmd));
	// _indexCmd.insert(std::pair<std::string, func>("KICK", &Server::_kickCmd));
	// _indexCmd.insert(std::pair<std::string, func>("MOTD", &Server::_motdCmd));
}

void	Server::_chooseCmd(User *user)
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
					_closeConnection(user);
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
				std::cout << WHITE "User <" << test.getNickname() << "> has joined <" << _channel[content].getName() << "> channel!" END << std::endl;
			}
		}
		else 
		{
			_channel.insert(std::pair<std::string, Channel>(content, Channel(test, content)));
			std::cout << WHITE "Channel <" << _channel[content].getName() << "> has been created" END << std::endl;
			_channel[content].setUsersMode(test.getNickname(), std::string("o"), 1);
		}
	}
	return ;
}

void	Server::_whoCmd(User *user, std::string param){
	if (!user->hasBeenWelcomed())
		return;
	try{
		Channel channel = _channel.at(param);
		std::map<std::string, User> channel_users = channel.getUsers();

		for (std::map<std::string, User>::iterator it = channel_users.begin(); it != channel_users.end(); it++){
			user->sendReply(RPL_WHOREPLY(user->getNickname(), param, it->second.getUsername(), it->second.getHostname(), it->second.getServer(), it->second.getNickname(), "", "", it->second.getRealname()));
		}
	}
	catch (const std::out_of_range &e){
		for (std::map<int, User*>::iterator it = _users.begin(); it != _users.end(); it++){
			if (it->second->getNickname() == param){
				user->sendReply(RPL_WHOREPLY(user->getNickname(), "*", it->second->getUsername(), it->second->getHostname(), it->second->getServer(), it->second->getNickname(), "", "", it->second->getRealname()));
				break;
			}
		}
	}
	user->sendReply(RPL_ENDOFWHO(user->getNickname(), param));
}

void	Server::_topicCmd(User *user, std::string param){
	std::string	channel_name;
	if (param.find(' ') != std::string::npos){
		channel_name = param.substr(0, param.find(' '));
		param = param.substr(param.find(' '), param.length());
	}
	else
		channel_name = param;
	user->sendReply("Nothing");
	// finir la fonction
}

void	Server::_changeModes(User *user, std::string target, std::string mode, bool value, bool isChannel){
	if (isChannel && _channel[target].checkUserIsOperatorOnChannel(user->getNickname()))
		_channel[target].setModes(mode, value);
	else if (isChannel && !_channel[target].checkUserIsOperatorOnChannel(user->getNickname()))
		return (user->sendReply(ERR_CHANOPRIVSNEEDED(user->getNickname(), target)));
	else
		user->setModes(mode, value);
}

void	Server::_modeCmd(User *user, std::string param){
	std::string target = param;
	bool		isChannel = false;

	if (param[0] == '#' || param[0] == '&')
		isChannel = true;
	if (param.find(' ') != std::string::npos){
		if (isChannel)
			target = param.substr(1, param.find(' '));
		else
			target = param.substr(0, param.find(' '));
		param = param.substr(param.find(' '), param.length());
	}
	else{
		if (isChannel)
			return(user->sendReply(RPL_CHANNELMODEIS(user->getNickname(), target, _channel[target].getChannelMode(), "")));
		else if (!isChannel && user->getNickname() == target)
			return(user->sendReply(RPL_UMODEIS(user->getNickname(), user->getUserMode())));
	}
	if (isChannel && !_checkChannelExistOnNetwork(target))
		return (user->sendReply(ERR_NOSUCHCHANNEL(user->getNickname(), target)));
	else if (!isChannel && !_checkUserExistOnNetwork(target))
		return (user->sendReply(ERR_NOSUCHNICK(user->getNickname(),target)));
	else if (!isChannel && user->getNickname() != target)
		return(user->sendReply(ERR_USERSDONTMATCH(user->getNickname())));
	for (uint  it = 0; it != param.length(); it++){
		if (param[it] == '-' && param[it] == '+'){
			uint it2 = it;
			while (param[it2] != 0 || (param[it2] != '+' && param[it2] != '-')){
				it2++;
			}
			if (it2 > it && param[it] == '-')
				_changeModes(user, target,param.substr(it + 1, it2), false, isChannel);
			else if (it2 > it && param[it] == '+')
				_changeModes(user, target,param.substr(it + 1, it2), true, isChannel);
		}
	}
}

void	Server::_listCmd(User *user, std::string param)
{
	// if (!user->hasBeenWelcomed)
	// 	return ;	
	if (param.empty())
	{
		for (std::map<std::string, Channel>::iterator it = _channel.begin(); it != _channel.end(); ++it)
		{
			std::stringstream ss;
			ss << it->second.getUsers().size();
			std::string userSize = ss.str();
			std::string buf = ":" + _hostname + " 322 " + user->getNickname() + " " + it->first + " " + userSize + " :" + it->second.getTopic() + "\r\n";
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
				std::stringstream ss;
				ss << _channel[*it].getUsers().size();
				std::string channelUserSize = ss.str();
				std::string buf = ":" + _hostname + " 322 " + user->getNickname() + " " + *it + " " + channelUserSize + " :" + _channel[*it].getTopic() + "\r\n";
				user->sendReply(buf);
			}
		}
	}	
	user->sendReply(":" + _hostname + " 323 " + user->getNickname() + " :End of /LIST\r\n");
}
 

void	Server::_noticeCmd(User *user, std::string param)
{
	if (!user->hasBeenWelcomed())
		return;
	if (param.find(':') == std::string::npos)
		return;
	if (param.find(':') == 0)
		return;
	std::string	msg = param.substr(param.find(':') + 1);
	std::string target = param.substr(0, param.find(' '));
	size_t		start = target.find_first_not_of(" ");

	target = target.substr(start, target.find_last_not_of(" ") - start + 1);
	if (target[0] == '#' || target[0] == '&')
	{
		std::cout << "channel" << std::endl;
		// channel
	 	// _channel[target].sendToAll(":" + user->getNickname() + "@IRC PRIVMSG " + target + " :" + msg + "\r\n");
		_channel[target].sendToAll(":" + user->getNickname() + "@IRC PRIVMSG " + target + " :" + msg + "\r\n");
		// try
		// {
		// 	Channel	*channel = _channel.at(target);

		// 	if (!channel->userIsIn(user) && channel->isNoOutside())
		// 		return;
		// 	channel->privmsg(user, msg);
		// }
		// catch (const std::out_of_range &e) {}
	}
	else
	{
		std::cout << "user" << std::endl;
		for (users_iterator it = _users.begin(); it != _users.end(); ++it)
		{
			if (it->second->getNickname() == target)
				return (it->second->sendReply(":" + user->getNickname() + "@IRC PRIVMSG " + target + " :" + msg + "\r\n"));
		}
	}
}