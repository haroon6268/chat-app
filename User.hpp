#include "string"
#include <mutex>

class User
{
  public:
    int id{};
    User(const std::string& _nick, int _sockfd);
    std::string nick;
    void sendMessage(const std::string& msg);
    void changeNick(std::string _nick);
    inline static int totalIds = 0;
    int sockfd{};
  private:
    std::mutex sendMutex;
};
