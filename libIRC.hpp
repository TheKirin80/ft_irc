#ifndef LIBIRC_HPP
#define LIBIRC_HPP

#define BLUE		"\033[1;34m"
#define RED			"\033[1;31m"
#define RESET		"\033[0m"

#define OK          0
#define NO_MATCH    0
#define MATCH       1
#define ERROR       1


#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <cctype>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>

#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

#endif