#pragma once

#include <cstdint>
#include <array>

enum MessageType : uint32_t
{
  INIT, 
  MESSAGE,
  CHANGE_NICK,
};

struct {
  uint32_t messageLength;
  MessageType type;
} typedef Header;

struct {
  Header header;
  char message[512];
} typedef Message;

constexpr int BUFSIZE = 4 + 512;
constexpr int headerSize = sizeof(uint32_t) + sizeof(MessageType);

void serializeMessage(const Message& message, std::array<char, BUFSIZE>& buf);
void deserializeMessage(const std::array<char, BUFSIZE>& buf, Message& message);
