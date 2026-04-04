#pragma once

#include "Room.hpp"
#include <unordered_map>

class Server{
  public:
    Server(std::string port);
    void startListening();
  private:
    std::unordered_map<std::string, Room> rooms;
    void init();
    std::string port{};
    int sockfd{};
    void handleRequest(int clientFd);
};
