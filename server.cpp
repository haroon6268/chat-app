#include "Server.hpp"
#include "serialize.hpp"
#include <thread>
#include <unistd.h>
#include <cerrno>
#include <arpa/inet.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

Server::Server(std::string port) : port{port}
{
  init(); 
  rooms.emplace("lobby", "lobby");
}

void Server::init()
{
   
  struct addrinfo hints{};
  struct addrinfo *servinfo;
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; //ASSIGNS ALL LOCAL ADDRESSES TO BE USABLE
  
  int status{};
  if((status = getaddrinfo(nullptr, port.c_str(), &hints, &servinfo)) != 0)
  {
    std::cerr << "getaddrinfo err::" << gai_strerror(status) << std::endl;
    exit(1);
  }
  
  sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
  
  if(bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
  {
    std::cerr << "bind failed" << std::endl;
    exit(1);
  }

  freeaddrinfo(servinfo);
    
}

void Server::startListening()
{
  if(listen(sockfd, SOMAXCONN) == -1)
  {
    std::cerr << "listen failed" << std::endl;
    exit(1);
  }

  struct sockaddr_storage recvAddr{};
  socklen_t addrSize{sizeof(recvAddr)};
  
  while(true)
  {
    int clientFd = accept(sockfd, (struct sockaddr *)&recvAddr, &addrSize);
    if(clientFd == -1)
    {
      std::cerr << "Accept failed" << std::endl;
      close(clientFd);
    }
    
    rooms["lobby"].addUser(clientFd);

    std::thread worker(&Server::handleRequest, this, clientFd);
    worker.detach();
  }
 
}


void Server::handleRequest(int clientFd)
{
  int totalReceived{};
  int curMessageLength{-1};
  
  std::vector<char> buf{};
  buf.resize(1024);
  std::string msg = "New user has joined the server!";
  rooms["lobby"].sendMessage(msg);
 
  while(true)
  {
    int status;
    status = recv(clientFd, buf.data(), 515, 0);

    if(status > 0)
    {
      totalReceived += status;
      if(curMessageLength == -1)
      {
        uint32_t messageLength{};
        std::memcpy(&messageLength, &buf[0], 4);
        curMessageLength = ntohl(messageLength);
      }
    
    //4 for message length header + message length to check if we have the complete data then extract
      if(totalReceived >= 4 + curMessageLength)
      {
        std::array<char, BUFSIZE> tempBuf{};
        std::copy(buf.begin(), buf.begin() + 4 + curMessageLength, tempBuf.begin());
        buf.erase(buf.begin(), buf.begin() + 4 + curMessageLength);
      
        Message message{};

        deserializeMessage(tempBuf, message);

        std::string str {message.message, static_cast<std::size_t>(curMessageLength)};

        rooms["lobby"].sendMessage(str);
        curMessageLength = -1;

     }

    }

    else if(status == 0)
    {
      std::cout << "Client disconnect..." << std::endl;
      close(clientFd);
      return;
    }

    else
    {
      std::cerr << "Error with recv::" << strerror(errno) << std::endl;
      close(clientFd);
      return;
    }

    
  }

}
