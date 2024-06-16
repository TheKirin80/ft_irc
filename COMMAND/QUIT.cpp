#include "libIRC.hpp"


void	Server::QUIT(int fd, std::string param)
{
	std::vector<Client>::iterator itclient = this->_list_client_serv.begin();
	std::vector<Channel>::iterator itchan = this->_list_channel_serv.begin();
    std::string part_param;
    Client client = this->getClientWithFd(fd);

	for (; itchan != this->_list_channel_serv.end() ; itchan++){
		if ((*itchan).in_list_client(client.getNickname()) == OK) // On verifie si le client est dans chacun des channels ce qui correspond a PART le channel
        {
            this->PART(fd, (*itchan).getName());
		}
	}
	//Gestion du message de sortie ou il faut ajouter le parametre param s'il n'est pas vide
	for (; itclient != this->_list_client_serv.end(); itclient++)
    {
		if ((*itclient).getFd() == fd){
			this->_list_client_serv.erase(itclient);
			break ;
		}
	}
	close(fd);
}