#include "libIRC.hpp"

void	Server::INVITE(int fd, std::string param){
	std::vector<std::string>	args;
    Client client = this->getClientWithFd(fd);

	if (client.getUserCheckState()== false)
    {
        //Message erreur user state
		return;
	}
	if (param.empty())
    {
		return ;
	}
	args = multi_split(param, " ");
	if (args.size() < 2)
    {
		//Error nombre param
		return ;
	}
	if (this->inListClientServ(args.at(0)) == ERROR)
    {
		//Message client inexistant
        return ;
	}
	if (client.in_channel(args.at(1)) == ERROR){
        //Message pas ton channel
		return ;
	}
	if (this->getChannelWithName(args.at(1)).in_list_client(args.at(0)) == OK)
    {
        //Message deja dans le channel
		return ;
	}
	//Message d'acceptation de l'INVITE
	this->getClientWithName(args.at(0)).add_channel(this->getChannelWithName(args.at(1)));
	this->getChannelWithName(args.at(1)).add_client(this->getClientWithName(args.at(0)));
	//voir pour un potentiel message !
}