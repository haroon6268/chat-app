#pragma once

#include <unordered_map>
#include <vector>

class Server{
  public:
    Server(std::string port);
    void startListening();
  private:
    std::unordered_map<int, std::vector<int>> rooms{};
    void init();
    std::string port{};
    int sockfd{};
    static void handleRequest(int clientFd);
};
