#include "../libIRC.hpp"

void	Server::TOPIC(int fd, std::string param)
{
	std::vector<std::string>	args;
    Client client = this->getClientWithFd(fd);

	if (client.getUserCheckState() == false)
    {
		this->sendErrMessage(fd, "USER not registered yet so command rejected\r\n");
		return ;
	}
	if (param.empty())
    {
        this->sendErrMessage(fd, ERR_NEEDMOREPARAMS(this->_name, client.getNickname(), "TOPIC"));
		return ;
	}
	args = once_split(param, " ");
	if (client.in_channel(args.at(0)) == ERROR)
    {
		this->sendErrMessage(fd, ERR_NOTONCHANNEL(this->_name, client.getNickname(), args.at(0)));
		return ;
	}
	if (this->getChannelWithName(args[0]).getRestrictedTopicState() == true) //Verification si le client est op en cas de restricted topic
    {
		if (this->getChannelWithName(args.at(0)).in_list_op_client(client.getNickname()) == ERROR)
        {
			this->sendErrMessage(fd, ERR_CHANOPRIVSNEEDED(this->_name, client.getNickname(), args.at(0)));
			return ;
		}
	}
	if (args.size() == 1)
    { // Cas ou l'on passe uniquement le channel sans topic, il faut afficher le topic s'il y en a un
		if (!(this->getChannelWithName(args.at(0)).getTopic().empty()))
        {
            this->sendRepMessage(fd, RPL_TOPIC(this->_name, client.getNickname(), args.at(0), this->getChannelWithName(args[0]).getTopic()));
		}
		else
        {
			this->sendRepMessage(fd, RPL_NOTOPIC(this->_name, client.getNickname(), args.at(0)));
		}
	} 
	else // Modification du topic avec annonce a tout les utilisateurs du channel via une alerte
    {
        this->getChannelWithName(args.at(0)).setTopic(args.at(1));
        this->getChannelWithName(args.at(0)).replyToAllTopic(client.getNickname(), this->_name, args.at(1));
	}
}