#pragma once

#include <cstdint>
#include <array>

struct {
  uint32_t messageLength;
} typedef Header;

struct {
  Header header;
  char message[512];
} typedef Message;

constexpr int BUFSIZE = 4 + 512;

void serializeMessage(const Message& message, std::array<char, BUFSIZE>& buf);
void deserializeMessage(const std::array<char, BUFSIZE>& buf, Message& message);
