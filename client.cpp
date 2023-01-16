#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdlib>
#include <filesystem>
constexpr int INVALID_SOCKET = -1;

#define SA struct sockaddr

constexpr int MAX				= 128;
constexpr int PORT				= 1234;
constexpr const char* IP_LOCAL	= "0.0.0.0";
constexpr const char* IP_SERVER = "192.168.0.214";

void recibir_mensaje(int sockfd, std::string& mensaje) {
	char buff[MAX];
	bzero(buff, MAX);
	read(sockfd, buff, sizeof(buff));
	mensaje = buff;
}

void enviar_mensaje(int sockfd, std::string& mensaje) {
	char buff[MAX];
	bzero(buff, MAX);

	const char* x = mensaje.c_str();
	write(sockfd, x, sizeof(buff));
}

void client_listener(int sockfd) {

	bool status = true;
	std::string commando;

	while (status) {
		recibir_mensaje(sockfd, commando);
		std::cout << commando;

		std::cin >> commando;
		if (commando == "stop")
			break;
		commando += "\r";
		enviar_mensaje(sockfd, commando);

		recibir_mensaje(sockfd, commando);
		std::cout << commando;
	}
}

int main() {
	int sockfd, connfd;

	struct sockaddr_in server;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == INVALID_SOCKET) {
		std::cout << "Creacion del socket fallida" << std::endl;
		return EXIT_FAILURE;
	}

	else
		std::cout << "Socket creado exitosamente" << std::endl;
	bzero(&server, sizeof(server));

	// asignar IP, PORT
	server.sin_family	   = AF_INET;
	server.sin_addr.s_addr = inet_addr(IP_SERVER);
	server.sin_port		   = htons(PORT);

	// connect
	if (connect(sockfd, (SA*)&server, sizeof(server)) != 0) {
		std::cout << "Conexion con el servidor fallo" << std::endl;
		close(sockfd);
		return EXIT_FAILURE;
	} else
		std::cout << "Conexion con el servidor exitosa" << std::endl;

	client_listener(sockfd);

	close(sockfd);
	return EXIT_SUCCESS;
}