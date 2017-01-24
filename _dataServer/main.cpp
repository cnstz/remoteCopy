#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h>
#include <iostream>
#include <pthread.h>
#include "server_helper.hpp"

#define CON 16

using namespace std;

void perror_exit(string);

int main(int argc, char **argv) {
	/* definitions & arguments */
	int port, thread_pool_size, queue_size;
	int sock, newsock;
	struct sockaddr_in server, client;
	socklen_t clientlen;
	struct sockaddr *serverptr = (struct sockaddr *) &server;
	struct sockaddr *clientptr = (struct sockaddr *) &client;
	struct hostent *rem;
	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-p") == 0) {
			port = atoi(argv[i + 1]);
		} else if (strcmp(argv[i], "-s") == 0) {
			thread_pool_size = atoi(argv[i + 1]);
		} else if (strcmp(argv[i], "-q") == 0) {
			queue_size = atoi(argv[i + 1]);
		}
	}
	cout << "Server's parameters are:\n\t port: " << port
			<< "\n\t thread_pool_size: " << thread_pool_size
			<< "\n\t queue_size: " << queue_size << endl;
	server_helper *helper = new server_helper(queue_size, thread_pool_size);
	cout << "Server was successfully initialized..." << endl;
	/***************************/

	/* open socket etc. */
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror_exit("socket");
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);
	if (bind(sock, serverptr, sizeof(server)) < 0) {
		perror_exit("bind");
	}
	if (listen(sock, CON) < 0) {
		perror_exit("listen");
	}
	/**********************/

	/* waiting for clients */
	cout << "Listening for connections to port: " << port << endl;
	while (1) {
		clientlen = sizeof(client);
		if ((newsock = accept(sock, clientptr, &clientlen)) < 0)
			perror_exit("accept");
		if ((rem = gethostbyaddr((char *) &client.sin_addr.s_addr,
				sizeof(client.sin_addr.s_addr), client.sin_family)) == NULL) {
			herror("gethostbyaddr");
			exit(1);
		}
		cout << "Accepted connection from: " << rem->h_name << endl;

		if (!helper->StartThread(newsock)) {
			// exit server
			close(sock);
			delete helper;
			return 0;
		}
	}
	/***********************/
}

void perror_exit(string message) {
	perror(message.c_str());
	exit(EXIT_FAILURE);
}

