#include "../libIRC.hpp"

void	Server::NICK(int fd, std::string param){

	Client client = this->getClientWithFd(fd);	//Verif passage de la commande PASS
	if (client.getPassCheckState() == false){
		return ;
	}
	//Verification si le param est vide si le client possede deja un nickname
	if (param.empty()){
		if (client.getNickname().empty()){
			//ERR_NONICKNAMEGIVEN
		}
		else
			//Affiche le nickname du client
		return ;
	}
	if (client.getUserCheckState() == false)
		if (this->checkNickname(param) == ERROR)
			return ;

	if (this->checkDoublon(fd, param) == ERROR)
		//Besoin d'un message d'erreur
		return ;

	if (client.getNickname() == param)
		return ;
	//Message au client
	client.setNickname(param);
	client.setNickCheckTrue();
}