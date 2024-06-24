#include "libIRC.hpp"

bool signal = false;

void Server::signalHandler(int sig)
{
	(void)sig;
	std::cout << BLUE << std::endl << "Signal Received!" << RESET << std::endl;
	signal = true;
}

void Server::init_server()
{
    struct sockaddr_in add;
    struct pollfd newPoll;
    int en = 1;
	add.sin_family = AF_INET;
	add.sin_port = htons(this->_port);
	add.sin_addr.s_addr = INADDR_ANY;
	this->_serv_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(this->_serv_fd == -1){
		std::cerr << RED << "Failed to create server socket" << RESET << std::endl;
		exit(1);
	}
	if(setsockopt(this->_serv_fd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1){
		std::cerr << RED << "Failed to set SO_REUSEADDR on socket" << RESET << std::endl;
		close(this->_serv_fd);
		exit(2);
	}
	if (fcntl(this->_serv_fd, F_SETFL, O_NONBLOCK) == -1){
		std::cerr << RED << "Failed to set O_NONBLOCK on socket" << RESET << std::endl;
		close(this->_serv_fd);
		exit(3);
	}
	if (bind(this->_serv_fd, (struct sockaddr *)&add, sizeof(add)) == -1){
		std::cerr << RED << "Failed to bind server socket" << RESET << std::endl;
		close(this->_serv_fd);
		exit(4);
	}
	if (listen(this->_serv_fd,  SOMAXCONN) == -1){
		std::cerr << RED << "Failed to listen" << RESET << std::endl;
		close(this->_serv_fd);
		exit(5);
	}
	newPoll.fd = this->_serv_fd;
	newPoll.events = POLLIN;
	newPoll.revents = 0;
	this->_list_fd.push_back(newPoll);
}

void Server::accept_new_client()
{
	struct sockaddr_in client_add;
	struct pollfd new_client_pollfd;
	memset(&client_add, 0, sizeof(client_add));
	socklen_t len = sizeof(client_add);
	int client_fd = accept(this->_serv_fd, (sockaddr *)&(client_add), &len);
	if (client_fd == -1){
		std::cout << RED << "Failed to accept() new client" << RESET << std::endl; 
		return;
	}
	if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1){
		std::cout << RED << "Failed to fcntl() new client" << RESET << std::endl;
		return;
	}
	Client client(client_fd);
	new_client_pollfd.fd = client_fd;
	new_client_pollfd.events = POLLIN;
	new_client_pollfd.revents = 0;
	this->_list_client_serv.push_back(client);
	this->_list_fd.push_back(new_client_pollfd);
	std::cout << BLUE << "Client <" << client_fd << "> Connected" << RESET << std::endl;
}
static std::vector<std::string> splitReceivedBuffer(std::string str)
{
	std::vector<std::string> ret;
	std::string line;
	size_t pos;

	while((pos = str.find("\r\n")) != std::string::npos)
	{
		line = str.substr(0, pos);
		ret.push_back(line);
		str = str.substr(pos + 2);
	}
	return (ret);
}

void Server::exec_command(int fd, std::string param)
{
	std::string	_list_cmd[12] = {"INVITE", "JOIN", "KICK", "PART", "TOPIC", "PRIVMSG", "NICK", "PASS", "USER", "CAP", "MODE", "QUIT"};
	std::vector<std::string> args = this->once_split(param, " ");
	if (args.size() == 1)
		args.push_back("");
	for (int i = 0; i < 12; i++)
	{
		if (args.at(0) == _list_cmd[i])
		{
			if (_list_cmd[i].compare("PASS") == 0)
				this->PASS(fd, args.at(1));
			else if (_list_cmd[i].compare("NICK") == 0)
				this->NICK(fd, args.at(1));
			else if (_list_cmd[i].compare("USER") == 0)
				this->USER(fd, args.at(1));
			else if (_list_cmd[i].compare("INVITE") == 0)
				this->INVITE(fd, args.at(1));
			else if (_list_cmd[i].compare("JOIN") == 0)
				this->JOIN(fd, args.at(1));
			else if (_list_cmd[i].compare("KICK") == 0)
				this->KICK(fd, args.at(1));
			else if (_list_cmd[i].compare("PART") == 0)
				this->PART(fd, args.at(1));
			else if (_list_cmd[i].compare("TOPIC") == 0)
				this->TOPIC(fd, args.at(1));
			else if (_list_cmd[i].compare("PRIVMSG") == 0)
				this->PRIVMSG(fd, args.at(1));
			else if (_list_cmd[i].compare("CAP") == 0)
				this->CAP();
			else if (_list_cmd[i].compare("MODE") == 0)
				this->MODE(fd, args.at(1));
			else if (_list_cmd[i].compare("QUIT") == 0)
				this->QUIT(fd, args.at(1));
			return ;
		}
	}
}

void Server::receive_new_data(int fd)
{
	std::vector<std::string> command;
	char buff[1024];
	memset(buff, 0, sizeof(buff));
	ssize_t ret = recv(fd, buff, sizeof(buff), 0);
	if(ret <= 0)
	{
		std::cout << RED << "Client <" << fd << "> Disconnected" << RESET << std::endl;
		this->getClientWithFd(fd).rm_all_channel();
		this->rm_client_serv(this->getClientWithFd(fd).getNickname());
		this->rm_fd(fd);
		close(fd);
	}
	else if (ret > 512) // Penser a le tester voir combien de caractere max retourner par IRSSI
	{
		memset(buff, '\0', 1024);
	}
	else
	{
		this->getClientWithFd(fd)._buff += buff;
		std::cout << "----" << this->getClientWithFd(fd)._buff << "----" << std::endl;
 		if(this->getClientWithFd(fd)._buff.find("\r\n") == std::string::npos)
			return;
		std::cout << "je suis la connard" << std::endl;
		if (this->getClientWithFd(fd)._buff == "\r\n"){
			this->getClientWithFd(fd)._buff.clear();
			return;
		}
		command = splitReceivedBuffer(this->getClientWithFd(fd)._buff);
		for (size_t i = 0; i < command.size() ; i++)
		{
			this->exec_command(fd, command[i]);
		}
		this->getClientWithFd(fd)._buff.clear();
	}
	memset(buff, 0, 1024);
}

void Server::loop_server()
{
	this->init_server();

	std::cout << BLUE << "Server <" << this->_name << "> Connected" << RESET << std::endl;
	std::cout << "Waiting to accept a connection...\n";
	while (signal == false)
	{
		if((poll(&_list_fd.at(0),this->_list_fd.size(),-1) == -1) && signal == false){
			std::cerr << RED << "Failed to poll()" << RESET << std::endl;
			return;
		}
		for (size_t i = 0; i < _list_fd.size(); i++)
		{
			if (this->_list_fd[i].revents & POLLIN) // revents permet de savoir si un event est detecte 	
			{
				if (this->_list_fd[i].fd == this->_serv_fd)
					this->accept_new_client();
				else
					this->receive_new_data(this->_list_fd[i].fd);
			}
		}
	}
	//close_fds();
}