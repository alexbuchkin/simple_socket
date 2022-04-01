#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd == -1) {
		perror("Unable to create socket");
		return 1;
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(12345);

	if (bind(socketfd, (const sockaddr*)&addr, sizeof(addr)) == -1) {
		perror("Failed to bind");
		return 1;
	}

	std::cerr << "Start listening\n";
	if (listen(socketfd, 1) == -1) {
		perror("Failed to listen");
		return 1;
	}

	int new_socketfd = accept(socketfd, nullptr, nullptr);
	std::cerr << "accepted!\n";
	const char msg[] = "aboba\n";
	/*
	while (true) {
		char buffer[100];
		memset(buffer, 0, sizeof(buffer));
		int bytes_read = recv(new_socketfd, (void*)buffer, sizeof(buffer), MSG_WAITALL);
		std::cerr << "bytes_read: " << bytes_read << '\n';
		if (bytes_read <= 0) {
			break;
		}

		std::cerr << buffer << '\n';
	}
	*/
	while (true) {
		int bytes_sent = send(new_socketfd, (const void*)msg, sizeof(msg), 0);
		if (bytes_sent <= 0) {
			perror("cannot send message");
			close(new_socketfd);
			break;
		}

		sleep(1);
	}

	if (close(socketfd) == -1) {
		perror("Failed to close socket");
		return 1;
	}
	return 0;
}
