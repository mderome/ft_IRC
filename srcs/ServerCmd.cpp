#include "../inc/user.hpp"
#include "../inc/server.hpp"

// CONNECTION  COMMANDES
void	Server::_indexingCmd(){
	_indexCmd.insert(std::pair<std::string, func>("PASS", &Server::pass_cmd));
	_indexCmd.insert(std::pair<std::string, func>("NICK", &Server::nick_cmd));
	_indexCmd.insert(std::pair<std::string, func>("PING", &Server::ping_cmd));
	_indexCmd.insert(std::pair<std::string, func>("OPER", &Server::nick_cmd));
	_indexCmd.insert(std::pair<std::string, func>("QUIT", &Server::quit_cmd));
}

void	Server::pass_cmd(User *user, std::string param){
	if (!param.length())
		return;//si pas de param  ERR_NEEDMOREPARAMS
	if (user->getRegistered())
		return; // throw Err ERR_ALREADYREGISTRED
	if (!param.compare(_password))//private value in server
		return;// ERR_PASSWDMISMATCH 
	user->setRegistered();
}

void	Server::nick_cmd(User *user, std::string param){
	if (param.empty())
		return; // ERR_NONICKNAMEGIVEN
	// parse for nickname is valid ERR_ERRONEUSNICKNAME
	std::map<int, User *>::iterator it;
	for (it = _users.begin(); it != _users.end(); it++){
		if (param.compare(it->second->getNickname()))
			return; // ERR_NICKCOLLISION
	}
	user->setNickname(param);
}

void	Server::ping_cmd(User *user, std::string param){
	if (param.empty())
		return ; //ERR_NEEDMOREPARAMS
	if (param != _hostname)
		return ; // ERR_NOSUCHSERVER
	// reply msg to RPL_PONG (nickname user,_hostname)
}

void	Server::oper_cmd(User *user, std::string param){
	// split param pour avoir name et password | Parameters: <name> <password>
	std::string	password;

	if (_hostname != user->getHostname())
		return ; // ERR_NOOPERHOST
	if (password != getPassword())
		return ; // ERR_PASSWDMISMATCH
	//RPL_YOUREOPER
	// avoir une fonction qui ajoute un operator a un channel
}

void	Server::quit_cmd(User *user, std::string param){
	// pas de parsing a faire
}