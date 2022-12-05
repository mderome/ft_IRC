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

void	Server::chooseCmd(User *user){
	std::string	msg = user->getMessage();
	std::string	param;
	std::string	cmd_name;

	//parse for msg for get cmd_name and param;
	// Bon courage :)
	std::cout << "Dans chooseCMD : " << msg << std::endl;
}

void	Server::user_cmd(User *user, std::string param){
	std::string	username;
	std::string	mode;
	// * jsp ce que c'est
	std::string	realname;

	std::cout << param << std::endl;
	if (user->getRegistered())
		return ; // ERR_ALREADYREGISTERED (462)
	// PArsing pour recuperer les params
	// check username realname et mode
	user->setRealname(username);
	user->setUsername(username);
	if (user->getNickname().length() && user->getRegistered() && !user->getDoWelcome())
		user->DoWelcome();
}

void	Server::pass_cmd(User *user, std::string param){
	if (!param.length())
		return;//si pas de param  ERR_NEEDMOREPARAMS
	if (user->getRegistered())
		return; // throw Err ERR_ALREADYREGISTRED
	if (!param.compare(_password))//private value in server
		return;// ERR_PASSWDMISMATCH 
	user->setRegistered();
	if (user->getNickname().length() && user->getUsername().length())
		user->DoWelcome();
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
	if (!user->getUsername().length() && user->getRegistered() && !user->getDoWelcome())
		user->DoWelcome();
}

void	Server::ping_cmd(User *user, std::string param){
	std::cout << user->getMessage() << std::endl;
	if (param.empty())
		return ; //ERR_NEEDMOREPARAMS
	if (param != _hostname)
		return ; // ERR_NOSUCHSERVER
	// reply msg to RPL_PONG (nickname user,_hostname)
}

void	Server::oper_cmd(User *user, std::string param){
	// split param pour avoir name et password | Parameters: <name> <password>
	std::string	password;

	std::cout << param << std::endl;
	if (_hostname != user->getHostname())
		return ; // ERR_NOOPERHOST
	if (password != getPassword())
		return ; // ERR_PASSWDMISMATCH
	//RPL_YOUREOPER
	// avoir une fonction qui ajoute un operator a un channel
}

void	Server::quit_cmd(User *user, std::string param){
	// pas de parsing a faire
	std::cout << user->getMessage() << std::endl;
	std::cout << param << std::endl;

}