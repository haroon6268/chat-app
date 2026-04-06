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

  uint8_t id;
  char nick[100];
} typedef Init;

union {
  char message[512];
  Init init;
} typedef MessageUnion;


struct {
  uint32_t messageLength;
  MessageType type;
} typedef Header;

struct {
  Header header;
  MessageUnion messageUnion;
} typedef Message;

//IF MESSAGE IS INIT, 8 bits for ID, then subtract 8 from message length to get nickname
constexpr int BUFSIZE = 4 + 512;
constexpr int headerSize = sizeof(uint32_t) + sizeof(MessageType);

void serializeMessage(const Message& message, std::array<char, BUFSIZE>& buf);
void deserializeMessage(const std::array<char, BUFSIZE>& buf, Message& message);
