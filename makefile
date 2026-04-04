all:
	g++ ChatServer.cpp serialize.cpp Room.cpp Server.cpp -I. -std=c++17 -o server && g++ ChatClient.cpp serialize.cpp -I. -o client -std=c++17
