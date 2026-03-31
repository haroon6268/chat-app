#include "serialize.hpp"

void serializeMessage(const Message& message, std::array<char, BUFSIZE>& buf)
{
  //CONVERT NUMERIC TYPES TO NETWORK ORDER
  uint16_t messageLength = htons(message.header.messageLength);

  //message length [2] -> message type [1] -> message[512]

  std::memcpy(&buf[0], &messageLength, 2);
  std::memcpy(&buf[2], &message.header.messageType, 1);
  std::memcpy(&buf[3], message.message,  message.header.messageLength);

}

void deserializeMessage(const std::array<char, BUFSIZE>& buf, Message& message)
{
  uint16_t messageLength{};
  
  std::memcpy(&messageLength, &buf[0], 2);
  messageLength = ntohs(messageLength);
  message.header.messageLength = messageLength;

  std::memcpy(&message.header.messageType, &buf[2], 1);

  std::memcpy(message.message, &buf[3], messageLength);

}

