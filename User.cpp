#include <sys/socket.h>
#include "User.hpp"
#include "serialize.hpp"

User::User(const std::string& _nick, int _sockfd) : nick{_nick}, sockfd{_sockfd}
{
  id = User::totalIds++;
}

void User::sendMessage(const std::string& msg)
{
  sendMutex.lock();

  Header header {static_cast<uint32_t>(msg.size()), MESSAGE};
  Message message{header};
  std::memcpy(message.messageUnion.message, msg.c_str(), msg.size());

  std::array<char, BUFSIZE> buf;
  serializeMessage(message, buf);

  send(sockfd, &buf[0], headerSize + message.header.messageLength, 0);

  sendMutex.unlock();
}

void User::changeNick(std::string _nick)
{
  nick = _nick; 
}
