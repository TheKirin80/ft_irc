#include "../libIRC.hpp"

void	Server::INVITE(int fd, std::string param){
	std::vector<std::string>	args;
    Client client = this->getClientWithFd(fd);

	if (client.getUserCheckState() == false)
    {
		this->sendErrMessage(fd, "USER not registered yet so command rejected\r\n");
		return ;
	}
	if (param.empty())
    {
        this->sendErrMessage(fd, ERR_NEEDMOREPARAMS(this->_name, client.getNickname(), "INVITE"));
		return ;
	}
	args = multi_split(param, " ");
	if (args.size() < 2)
    {
		this->sendErrMessage(fd, ERR_NEEDMOREPARAMS(this->_name, client.getNickname(), "INVITE"));
		return ;
	}
	if (this->inListClientServ(args.at(0)) == ERROR)
    {
		this->sendErrMessage(fd, ERR_NOSUCHNICK(this->_name, client.getNickname(), args.at(0)));
        return ;
	}
	if (client.in_channel(args.at(1)) == ERROR)
	{
        this->sendErrMessage(fd, ERR_NOTONCHANNEL(this->_name, client.getNickname(), args.at(1)));
		return ;
	}
	if (this->getChannelWithName(args.at(1)).in_list_client(args.at(0)) == OK)
    {
        this->sendErrMessage(fd, ERR_USERONCHANNEL(this->_name, client.getNickname(), args.at(0), args.at(1)));
		return ;
	}
	this->sendRepMessage(fd, RPL_INVITING(this->_name, client.getNickname(), args.at(0), args.at(1)));
	this->sendRepMessage(this->getClientWithName(args.at(0)).getFd(), INVITE_INFO(client.getNickname(), client.getUsername(), this->_name, args.at(0), args.at(1)));
	this->getClientWithName(args.at(0)).add_channel(this->getChannelWithName(args.at(1)));
	this->getChannelWithName(args.at(1)).add_client(this->getClientWithName(args.at(0)));
	this->replysExistChannel(this->getClientWithName(args.at(0)).getFd(), args.at(1));
}