#include "../inc/user.hpp"
#include "../inc/server.hpp"

// CONNECTION  COMMANDES
void	Server::_indexingCmd(){
	_indexCmd.insert(std::pair<std::string, func>("USER", &Server::_userCmd)); 
	_indexCmd.insert(std::pair<std::string, func>("PASS", &Server::_passCmd));
	_indexCmd.insert(std::pair<std::string, func>("NICK", &Server::_nickCmd));
	_indexCmd.insert(std::pair<std::string, func>("CAP LS", &Server::_caplsCmd));
	// _indexCmd.insert(std::pair<std::string, func>("PING", &Server::_pingCmd));
	_indexCmd.insert(std::pair<std::string, func>("QUIT", &Server::_quitCmd));
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

void	Server::_caplsCmd(User *user, std::string buf)
{
	if (buf != "LS")
		return (user->sendReply("CAP LS command"));
}

void	Server::_userCmd(User *user, std::string buf)
{
	if (user->hasBeenWelcomed())
		return (user->sendReply("Error: user: already welcomed"));
	if (buf.empty())
		return (user->sendReply("Error: user: empty"));

	// parsing here

	if (user->getNickname().size() && user->getPassword() && !user->hasBeenWelcomed())
		user->welcome();
}

void	Server::_passCmd(User *user, std::string buf)
{
	if (user->hasBeenWelcomed())
		return (user->sendReply("Error: pass: already welcomed"));
	if (!buf.length())
		return (user->sendReply("Error: pass: empty"));
	if (buf.compare(_password))
	{
		user->setPassword(false);
		return (user->sendReply("Error: pass: wrong password"));
	}
	user->setPassword(true);
	if (user->getNickname().length() && user->getUser().length())
		user->welcome();
}

void	Server::_nickCmd(User *user, std::string buf)
{
	if (buf.empty())
		return (user->sendReply("Error: nick: empty"));
	if (buf.find(' ') != std::string::npos)
		buf = buf.substr(0, buf.find_first_of(' '));
	for (users_iterator it = _users.begin(); it != _users.end(); ++it)
	{
		if (it->second->getNickname() == buf)
			return (user->sendReply("Error: nick: already used"));
	}
	user->setNickname(buf);
	if (user->getUser().length() && user->getPassword() && !user->hasBeenWelcomed())
		user->welcome();
}

void	Server::_quitCmd(User *user, std::string param){
	// pas de parsing a faire
	std::cout << user->getMessage() << std::endl;
	std::cout << param << std::endl;
}

// void	Server::_pingCmd(User *user, std::string param){
// 	std::cout << user->getMessage() << std::endl;
// 	if (param.empty())
// 		return ; //ERR_NEEDMOREPARAMS
// 	if (param != _hostname)
// 		return ; // ERR_NOSUCHSERVER
// 	// reply msg to RPL_PONG (nickname user,_hostname)
// }

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