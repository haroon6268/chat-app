#include <string>
#include <mutex>
#include <set>

class Room
{
  public:
    Room(std::string roomName);
    Room();
    int id;
    std::string name{};
    void addUser(int userFd);
    void removeUser(int userFd);
    void sendMessage(std::string message);
    
  private:
    inline static int roomId = 0;
    std::set<int> sockfds{};
    std::mutex sendMutex{};
};
