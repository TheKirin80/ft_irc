#include "../libIRC.hpp"

void	Server::PART(int fd, std::string param)
{
	std::vector<std::string>	args;
	std::string					part_message = "";
    Client client = this->getClientWithFd(fd);

	if (client.getUserCheckState() == false)
    {
		this->sendErrMessage(fd, "USER not registered yet so command rejected\r\n");
		return ;
	}
	if (param.empty())
    {
        this->sendErrMessage(fd, ERR_NEEDMOREPARAMS(this->_name, client.getNickname(), "PART"));
		return ;
	}
	args = once_split(param, " "); // permet de separer les channels a quitter du potentiel message de sortie
	if (args.size() > 1)
    {
		part_message = args[1];
	}
	args = multi_split(args[0], ","); // On separe les differents channels
    std::vector<std::string>::iterator it = args.begin();
	for (; it != args.end(); it++) // Verification si les channels existent
    {
		if (this->inListChannelServ(*it) == ERROR)
        {
			this->sendErrMessage(fd, ERR_NOSUCHCHANNEL(this->_name, client.getNickname(), (*it)));
			return ;
		}
	}
    it = args.begin();
	for (; it != args.end(); it++) // Verification si le client appartient aux differents channels
    {
		if (client.in_channel(*it) == ERROR)
        {
			this->sendErrMessage(fd, ERR_NOTONCHANNEL(this->_name, client.getNickname(), (*it)));
			return ;
		}
	}
    it = args.begin();
	for (; it != args.end(); it++) // On supprime le channel du client et le client du channel et on supprime le channel si celui ci est vide
    {
		if (part_message == "")
			this->getChannelWithName(*it).replyToAll(PART_INFO(client.getNickname(), client.getUsername(), this->_name, (*it), ""));
		else
			this->getChannelWithName(*it).replyToAll(PART_INFO(client.getNickname(), client.getUsername(), this->_name, (*it), " " + part_message));
		client.rm_channel((*it));
		this->getChannelWithName((*it)).rm_client(client);
		if (this->getChannelWithName((*it)).isChannelEmpty() == OK)
        {
			rm_channel_serv((*it));
        }
    }
}
