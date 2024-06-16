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

void	Server::rm_channel_serv(std::string channel)
{
	std::vector<Channel>::iterator it = this->_list_channel_serv.begin();
	for (; it != this->_list_channel_serv.end() ; it++)
	{
		if (channel == (*it).getName())
		{
			_list_channel_serv.erase(it);
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
			_list_client_serv.erase(it);
			return ;
		}
	}
}

int	Server::checkNickname(std::string param)
{
	// std::cout << param.get_params() << std::endl;
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

int	Server::inListChannelServ(const std::string name_chan)
{
	std::vector<Channel>::iterator it = this->_list_channel_serv.begin();

	for (; it != this->_list_channel_serv.end(); it++){
		if (name_chan == (*it).getName())
			return (OK);
	}
	return (ERROR);
}