#include <sys/types.h>
#include <cerrno>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <array>
#include <string>
#include <unistd.h>
#include "serialize.hpp"

int main(int argc, char* argv[])
{
  if(argc != 2)
  {
    std::cout << "Usage: ./<app_name> <port_number>" << std::endl;
    exit(1);
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


  if(connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1)
  {
    std::cerr << "connect failed::" << strerror(errno) << std::endl;
    exit(1);
  }
  
  freeaddrinfo(servinfo);

  std::array<char, 1024> buf{};
  
  std::string message{};

  int size = recv(sockfd, buf.data(), buf.size(), 0);
  message.append(buf.data(), size);

  std::cout << message << std::endl;
  
  while(true)
  {
    Message message {};
    
    std::cout << "Enter Message: ";
    
    std::string input{};
    std::getline(std::cin, input);

    if(input.length() >= 512)
    {
      std::cout << "Message must be less than 512 characters!" << std::endl;
      continue;
    }
    
    std::memcpy(message.message, input.c_str(), input.length());
    
    message.header.messageLength = input.length();

    message.header.messageType = CHAT;

    int messageSize = 3 + message.header.messageLength; // 3 bytes for message length + messageType then length the mesasge

    std::array<char, BUFSIZE> buf{};
    
    serializeMessage(message, buf);

    send(sockfd, buf.data(), messageSize, 0);

  }
  
  close(sockfd);
  exit(0);




  
}
