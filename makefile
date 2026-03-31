all:
	g++ server.cpp serialize.cpp -I. -o server && g++ client.cpp serialize.cpp -I. -o client
