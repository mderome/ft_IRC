#include "../inc/User.hpp"

void	pass_cmd(User *user, std::string param){
	if (!param.length())
		return;//si pas de param  ERR_NEEDMOREPARAMS
	if (user->getRegistered())
		return; // throw Err ERR_ALREADYREGISTRED
	if (!param.compare(_password))//private value in server
		return;// ERR_PASSWDMISMATCH 
	user->setRegister();
}

void	nick_cmd(User *user, std::string param){
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

void	ping_cmd(User *user, std::string param){
	if (param.empty())
		return ; //ERR_NEEDMOREPARAMS
	if (param != _hostname)
		return ; // ERR_NOSUCHSERVER
	// reply msg to RPL_PONG (nickname user,_hostname)
}

void	oper_cmd(User *user, std::string param){
	// split param pour avoir name et password | Parameters: <name> <password>
	if (this->getHostname() != user->getHostname())
		return ; // ERR_NOOPERHOST
	if (password != getPassword())
		return ; // ERR_PASSWDMISMATCH
	//RPL_YOUREOPER
	// avoir une fonction qui ajoute un operator a un channel
}
