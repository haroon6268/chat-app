#include <cstddef>
#include <cstring>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <array>
#include <netdb.h>
#include <cerrno>
#include <iostream>
#include <unistd.h>
#include <cstdint>
#include <thread>
#include "serialize.hpp"

void handleRequest(int clientFd);

int main(int argc, char* argv[])
{

  if(argc != 2)
  {
    std::cout << "Usage: ./<app_name> <port_number>" << std::endl;
    exit(1);
  }
  
  int port;

  try
  {
    port = std::stoi(argv[1]); 
  }
  catch(const std::exception& e)
  {
    std::cerr << "Invalid port: " << e.what() << std::endl;
  }

  struct addrinfo hints{};
  struct addrinfo *servinfo;
  
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; //ASSIGNS LOCAL HOST TO SOCKET
 
  int status{};
  if((status = getaddrinfo(nullptr, argv[1], &hints, &servinfo)) != 0)
  {
    std::cerr << "getaddrinfo err::" << gai_strerror(status) << std::endl;
    exit(1);
  }
  
  int sockfd {socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)};
  
  if(bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
  {
    std::cerr << "bind failed" << std::endl;
    exit(1);
  }

  freeaddrinfo(servinfo);

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
      exit(1);
    }
    std::thread worker(handleRequest, clientFd);
    worker.detach();
  }
  
  
  close(sockfd);
  
  exit(1);


}


void handleRequest(int clientFd)
{
  int totalReceived{};
  int curMessageLength{-1};

  std::vector<char> buf{};
  buf.resize(1024);
  std::string msg = "Welcome to the Chat Server!";
  send(clientFd, msg.c_str(), msg.length(), 0);
 
  while(true)
  {
    int status;
    status = recv(clientFd, buf.data(), 515, 0);

    if(status > 0)
    {
      totalReceived += status;
      if(curMessageLength == -1)
      {
        uint16_t messageLength{};
        std::memcpy(&messageLength, &buf[0], 2);
        curMessageLength = ntohs(messageLength);
      }
    
    //3 for message length and message type. + to curMessage length to check if we have the complete data then extract
      if(totalReceived >= 3 + curMessageLength)
      {
        std::array<char, BUFSIZE> tempBuf{};
        std::copy(buf.begin(), buf.begin() + 3 + curMessageLength, tempBuf.begin());
        buf.erase(buf.begin(), buf.begin() + 3 + curMessageLength);
      
        Message message{};

        deserializeMessage(tempBuf, message);

        std::string str {message.message, static_cast<std::size_t>(curMessageLength)};

        std::cout << str << std::endl;

        curMessageLength = -1;

     }

    }

    else if(status == 0)
    {
      std::cout << "Client disconnect..." << std::endl;
    }

    else
    {
      std::cerr << "Error with recv::" << strerror(errno) << std::endl;
      close(clientFd);
      exit(1);
    }

    
  }

}
