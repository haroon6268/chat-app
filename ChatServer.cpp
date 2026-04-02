#include "Server.hpp"

void handleRequest(int clientFd);

int main(int argc, char* argv[])
{
  Server server{"5050"};
  server.startListening();
}
