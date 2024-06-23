#include "libIRC.hpp"

bool Server::signal = false;

void Server::signalHandler(int sig)
{
	(void)sig;
	std::cout << BLUE << std::endl << "Signal Received!" << RESET << std::endl;
	Server::signal = true;
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
	new_newPoll.revents = 0;
	this->_list_fd.push_back(newPoll);
}

void Server::accept_new_client()
{
	struct sockaddr_in client_add;
	struct pollfd new_client_pollfd;
	memset(&client_add, 0, sizeof(client_add));
	socklen_t len = sizeof(client_add);
	int client_fd = accept(server_fdsocket, (sockaddr *)&(client_add), &len);
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
	this->_server_fd.push_back(new_client_pollfd);
	std::cout << BLUE << "Client <" << client_fd << "> Connected" << RESET << std::endl;
}
void Server::receive_new_data(int fd)
{
	//  Va servir a traiter les requetes du client 
}

void Server::loop_server(int port, std::string passwrd)
{
	this->_passwrd_serv = passwrd;
	this->_port = port;
	this->init_server();

	std::cout << BLUE << "Server <" << this->_name << "> Connected" << RESET << std::endl;
	std::cout << "Waiting to accept a connection...\n";
	while (Server::signal == false)
	{
		if((poll(&this->_list_fd.at(0),this->_list_fd.size(),-1) == -1) && Server::signal == false){
			std::cerr << RED << "Failed to poll()" << RESET << std::endl;

		}
		for (size_t i = 0; i < this->_list_fd.size(); i++)
		{
			if (this->_list_fd.at(i).revents & POLLIN) // revents permet de savoir si un event est detecte 	
			{
				if (this->_list_fd.at(i).fd == _serv_fd)
					this->accept_new_client();
				else
					this->receive_new_data(this->_list_fd.at(i).fd);
			}
		}
	}
	close_fds();
}