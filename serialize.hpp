#pragma once

#include <cstdint>
#include <array>

struct {
  uint16_t messageLength;
  uint8_t messageType;
} typedef Header;

struct {
  Header header;
  char message[512];
} typedef Message;

enum MessageType
{
  CHAT
};

constexpr int BUFSIZE = 2 + 1 + 512;

void serializeMessage(const Message& message, std::array<char, BUFSIZE>& buf);
void deserializeMessage(const std::array<char, BUFSIZE>& buf, Message& message);
