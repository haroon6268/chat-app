#include <sys/types.h>
#include <cerrno>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <array>
#include <string>
#include <thread>
#include <unistd.h>
#include "serialize.hpp"

void handleReceivedMessages(int sockfd)
{
int totalReceived{};
int curMessageLength{-1};

std::vector<char> buf{};
buf.resize(1024);

while(true)
{
  int status;
  status = recv(sockfd, buf.data() + totalReceived, 515, 0);

  if(status > 0)
  {
    totalReceived += status;
    if(curMessageLength == -1 and totalReceived >= 4)
    {
      uint32_t messageLength{};
      std::memcpy(&messageLength, &buf[0], 4);
      curMessageLength = ntohl(messageLength);
    }
  
  //4 for message length header + message length to check if we have the complete data then extract
    if(curMessageLength != -1 && totalReceived >= headerSize + curMessageLength)
    {
      std::array<char, BUFSIZE> tempBuf{};
      std::copy(buf.begin(), buf.begin() + headerSize + curMessageLength, tempBuf.begin());
      buf.erase(buf.begin(), buf.begin() + headerSize + curMessageLength);
      buf.resize(1024);
    
      Message message{};

      deserializeMessage(tempBuf, message);

      std::string str {message.message, static_cast<std::size_t>(curMessageLength)};
      std::cout << str << std::endl;

      totalReceived -= headerSize + curMessageLength;
      curMessageLength = -1;
      
   }

  }

  else if(status == 0)
  {
    std::cout << "Client disconnect..." << std::endl;
    close(sockfd);
    return;
  }

  else
  {
    std::cerr << "Error with recv::" << strerror(errno) << std::endl;
    close(sockfd);
    return;
  }

  
}


}


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
  if((status = getaddrinfo("127.0.0.1", argv[1], &hints, &servinfo)) != 0)
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

  std::thread worker(handleReceivedMessages, sockfd);
  
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
    message.header.type = MESSAGE;

    int messageSize = headerSize + message.header.messageLength; 

    std::array<char, BUFSIZE> buf{};
    
    serializeMessage(message, buf);

    send(sockfd, buf.data(), messageSize, 0);

  }
  
  close(sockfd);
  exit(0);




  
}
