#include "libIRC.hpp"

Server::Server(int port, std::string passwrd) : _name("ft_irc"), _port(port), _passwrd_serv(passwrd)
{
    return;
}
Server::~Server(void)
{
    this->_list_channel_serv.clear();
    this->_list_client_serv.clear();
    return;
}
//_name
std::string Server::getName(void) const
{
    return (this->_name);
}
//_port
int Server::getPort(void) const
{
    return (this->_port);
}
//_passwrd_serv
std::string Server::getPasswrdServ(void) const
{
    return (this->_passwrd_serv);
}

Client &Server::getClientWithName(std::string client)
{
    std::vector<Client>::iterator it = this->_list_client_serv.begin();
	
	for (; it != this->_list_client_serv.end(); it++){
		if (client == (*it).getNickname())
			return ((*it));
	}
	return ((*it));
}
Client &Server::getClientWithFd(int fd)
{
    std::vector<Client>::iterator it = this->_list_client_serv.begin();
	
	for (; it != this->_list_client_serv.end(); it++){
		if (fd == (*it).getFd())
			return ((*it));
	}
	return ((*it));
}

Channel     &Server::getChannelWithName(std::string channel)
{
	std::vector<Channel>::iterator it = this->_list_channel_serv.begin();
	
	for (; it != this->_list_channel_serv.end(); it++){
		if (channel == (*it).getName())
			return ((*it));
	}
	return ((*it));
}
//utilitaire static
static int special_char(char c){
	if (c == '\\' ||  c == '|' ||  c == '`' ||  c == ',' || c == '^' || c == '_' || c == '[' || c == ']' || c == '{' || c == '}' )
		return (MATCH);
	return (NO_MATCH);
}

//utilitaire
void	Server::add_channel_serv(int fd, std::string name)
{
	//Creation du nouveau channel
	Channel new_channel = Channel(name);
	this->_list_channel_serv.push_back(new_channel);
	//Ajout du client au liste classique et op ainsi qu'ajout du channel sur le client 
	Client  client = this->getClientWithFd(fd);
	Channel channel = this->getChannelWithName(name); // Passe la reference du channel dans le vector channel du serv
	channel.add_client(client);
	channel.add_op_client(client);
	client.add_channel(channel);
}
void	Server::rm_channel_serv(std::string channel)
{
	std::vector<Channel>::iterator it = this->_list_channel_serv.begin();
	for (; it != this->_list_channel_serv.end() ; it++)
	{
		if (channel == (*it).getName())
		{
			this->_list_channel_serv.erase(it);
			return;
		}
	}
}
void	Server::rm_client_serv(std::string client)
{
	std::vector<Client>::iterator it = this->_list_client_serv.begin();
	for (; it != _list_client_serv.end() ; it++)
	{
		if (client == (*it).getNickname())
		{
			this->_list_client_serv.erase(it);
			return ;
		}
	}
}

int	Server::checkNickname(std::string param)
{
	if (param.size() > 9)
    {	
		return (ERROR);
	}
	if (special_char(param.at(0)) == MATCH && isalpha(param.at(0)) == NO_MATCH)
    {
		return (ERROR);
	}
	for (size_t i = 1; i < param.size(); i++){
		if (special_char(param.at(i)) == NO_MATCH && isalpha(param.at(i)) == NO_MATCH && isdigit(param.at(i)) == NO_MATCH && (param.at(i)) != '-')
        {
			return (ERROR);
		}
	}
	return (OK);
}

int Server::checkDoublon(int fd, std::string param)
{
    std::vector<Client>::iterator it = this->_list_client_serv.begin();
	
	for (; it != this->_list_client_serv.end(); it++){
		if (param == (*it).getNickname() && fd != (*it).getFd())
			return (ERROR);
	}
	return (OK);
}
std::vector<std::string> Server::multi_split(std::string param, std::string lim) // Utiliser pour la commande USER entre autre
{
	std::vector<std::string>	ret;
	std::string::size_type		found;	
	
	if (param.empty())
		return ret;
	found = param.find(lim);
	while (found != std::string::npos){
		if (found != 0)
			ret.push_back(param.substr(0, found));
		param = param.substr(found + lim.size(), param.size());
		found = param.find(lim);
	}
	if (param.size() != 0)
		ret.push_back(param);

	return (ret);
}

std::vector<std::string> Server::once_split(std::string param, std::string lim) // Utiliser pour sortir la commande des parametres
{
	std::vector<std::string>	ret;
	std::string::size_type		found;	
	
	if ( (found = param.find(lim)) != std::string::npos){
		if (found != 0)
			ret.push_back(param.substr(0, found));
		param = param.substr(found + lim.size(), param.size());
	}
	if (param.size() != 0)
		ret.push_back(param);

	return (ret);
}

int	Server::inListChannelServ(std::string name_chan)
{
	std::vector<Channel>::iterator it = this->_list_channel_serv.begin();

	for (; it != this->_list_channel_serv.end(); it++){
		if (name_chan == (*it).getName())
			return (OK);
	}
	return (ERROR);
}
int	Server::checkChannelName(std::string name) 
{
	std::string to_send;
	
	if ((name[0] != '&' && name[0] != '#' && name[0] != '+' && name[0] != '!') || name.size() > 50 )
	{
		//message error
		return (ERROR);
	}
	for (size_t i = 1; name[i]; i++){
		if (name[i] == ' ' || name[i] == ',' || name[i] == 7)
		{
			//message error
			return (ERROR);
		}
	}	
	return (OK);
}

int	Server::inListClientServ(std::string name_client)
{
	std::vector<Client>::iterator it = this->_list_client_serv.begin();

	for (; it != this->_list_client_serv.end(); it++){
		if (name_client == (*it).getNickname())
			return (OK);
	}
	return (ERROR);
}

void Server::rm_fd(int fd)
{
	std::vector<struct pollfd>::iterator it = this->_list_fd.begin();
	for (; it != this->_list_fd.end(); it++)
	{
		if ((*it).fd == fd)
		{
			this->_list_fd.erase(it);
			return;
		}
	}
}

// void	Server::close_fd()
// {
// 	for(size_t i = 0; i < clients.size(); i++){
// 		std::cout << RED << "Client <" << clients[i].GetFd() << "> Disconnected" << WHI << std::endl;
// 		close(clients[i].GetFd());
// 	}
// 	if (server_fdsocket != -1){	
// 		std::cout << RED << "Server <" << server_fdsocket << "> Disconnected" << WHI << std::endl;
// 		close(server_fdsocket);
// 	}
// }