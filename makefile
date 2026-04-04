all:
	g++ ChatServer.cpp serialize.cpp Room.cpp User.cpp Server.cpp -I. -std=c++20 -o server && g++ ChatClient.cpp serialize.cpp -I. -o client -std=c++20
