#include "../libIRC.hpp"

void	Server::PASS(int fd, std::string param){
	
	Client client = this->getClientWithFd(fd);
	if (param.empty())
	{
		this->sendErrMessage(fd, ERR_NEEDMOREPARAMS(this->_name, client.getNickname(), "PASS"));
		return ;
	}
	if (client.getUserCheckState() == true)
	{
		return ;
	}
	if (client.getPassCheckState() == true)
	{
		this->sendErrMessage(fd, ERR_ALREADYREGISTERED(this->_name, client.getNickname()));
		return ;
	}
	// if (client.getNickCheckState() == true){
	// 	return ;
	// }
	if (param != this->_passwrd_serv)
	{
		this->sendErrMessage(fd, ERR_PASSWDMISMATCH(this->_name, client.getNickname()));
		return ;
	}
	this->getClientWithFd(fd).setPassCheckTrue();
}