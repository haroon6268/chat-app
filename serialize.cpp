#include "serialize.hpp"

void serializeMessage(const Message& message, std::array<char, BUFSIZE>& buf)
{
  //CONVERT NUMERIC TYPES TO NETWORK ORDER
  uint32_t messageLength = htonl(message.header.messageLength);
  uint32_t  messageType = htonl(message.header.type);

  //message length [4] -> message[512]

  std::memcpy(&buf[0], &messageLength, 4);
  std::memcpy(&buf[4], &messageType, 4); 
  std::memcpy(&buf[8], message.message,  message.header.messageLength);

}

void deserializeMessage(const std::array<char, BUFSIZE>& buf, Message& message)
{
  uint32_t messageLength{};
  uint32_t messageType{};
  
  std::memcpy(&messageLength, &buf[0], 4);
  messageLength = ntohl(messageLength);
  message.header.messageLength = messageLength;

  std::memcpy(&messageType, &buf[4], 4);
  messageType = ntohl(messageType);
  message.header.type = static_cast<MessageType>(messageType);
  std::memcpy(message.message, &buf[8], messageLength);

}

