all:
	g++ ChatServer.cpp serialize.cpp Server.cpp -I. -o server && g++ ChatClient.cpp serialize.cpp -I. -o client
