#include "serialize.hpp"

void serializeMessage(const Message& message, std::array<char, BUFSIZE>& buf)
{
  //CONVERT NUMERIC TYPES TO NETWORK ORDER
  uint32_t messageLength = htonl(message.header.messageLength);
  uint32_t  messageType = htonl(message.header.type);

  //message length [4] -> message[512]

  std::memcpy(&buf[0], &messageLength, 4);
  std::memcpy(&buf[4], &messageType, 4); 
  
  switch(message.header.type)
  {
    case(MESSAGE):
      std::memcpy(&buf[8], message.messageUnion.message,  message.header.messageLength);
      break;
    case(INIT):
      std::memcpy(&buf[8], &message.messageUnion.init.id, 1);
      std::memcpy(&buf[9], message.messageUnion.init.nick, message.header.messageLength - 1);
      break;
    case(CHANGE_NICK):
      break; 
  }

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

  switch(message.header.type)
  {
    case(MESSAGE):
      std::memcpy(message.messageUnion.message, &buf[8], messageLength);
      break;
    case(INIT):
      std::memcpy(&message.messageUnion.init.id, &buf[8], 1);
      std::memcpy(message.messageUnion.init.nick, &buf[9], messageLength - 1);
      break;
    case(CHANGE_NICK):
      break;
  }

}

