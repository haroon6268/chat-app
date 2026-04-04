#include "serialize.hpp"

void serializeMessage(const Message& message, std::array<char, BUFSIZE>& buf)
{
  //CONVERT NUMERIC TYPES TO NETWORK ORDER
  uint32_t messageLength = htonl(message.header.messageLength);

  //message length [4] -> message[512]

  std::memcpy(&buf[0], &messageLength, 4);
  std::memcpy(&buf[4], message.message,  message.header.messageLength);

}

void deserializeMessage(const std::array<char, BUFSIZE>& buf, Message& message)
{
  uint32_t messageLength{};
  
  std::memcpy(&messageLength, &buf[0], 4);
  messageLength = ntohl(messageLength);
  message.header.messageLength = messageLength;


  std::memcpy(message.message, &buf[4], messageLength);

}

