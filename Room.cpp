#include "Room.hpp"
#include <sys/socket.h>
#include "serialize.hpp"

Room::Room(std::string roomName) : id{roomId++}, name{roomName}
{
}

Room::Room()
{
  int curid = roomId++;

  id = curid;
  name = std::to_string(curid);
}

void Room::addUser(int userFd)
{
  //fix race conditions here
  sockfds.insert(userFd);
}

void Room::removeUser(int userFd)
{

  //fix race conditions
  sockfds.insert(userFd);
}


void Room::sendMessage(std::string message)
{
  sendMutex.lock();
  for(const auto& sockfd : sockfds)
  {
    Header header {static_cast<uint32_t>(message.size())};
    Message msg{header};
    std::memcpy(msg.messageUnion.message, message.c_str(), message.size());
    
    std::array<char, BUFSIZE> buf;
    serializeMessage(msg, buf);

    send(sockfd, &buf[0], 4 + msg.header.messageLength, 0);
  }
  sendMutex.unlock();
}
