#pragma once

#include "Room.hpp"
#include <unordered_map>
#include "User.hpp"
#include <unordered_set>
#include <memory>

class Server{
  public:
    Server(std::string port);
    void startListening();
    bool doesNickExist(const std::string& nick);
  private:
    std::unordered_map<std::string, Room> rooms;
    void init();
    std::string port{};
    int sockfd{};
    void handleRequest(int clientFd);
    std::unordered_map<int, std::unique_ptr<User>> allUsers{};
    std::unordered_set<std::string> allNicks{};
    int addUser(std::string nick, int sockfd);
    bool addNick(std::string nick);
    void removeNick(std::string nick);
    std::mutex userMutex;
    std::mutex nickMutex;
};
