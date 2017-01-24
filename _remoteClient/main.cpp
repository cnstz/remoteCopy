#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h>
#include <iostream>
#include <sys/ioctl.h>
#include <fcntl.h>
#include "functions.hpp"

using namespace std;

int main(int argc, char *argv[]) {
	/* definitions & arguments */
	int server_port;
	char cserver_ip[100];
	string server_ip;
	string directory;
	int sock;
	struct sockaddr_in server;
	struct sockaddr *serverptr = (struct sockaddr*) &server;
	struct hostent *rem;
	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-i") == 0) {
			strcpy(cserver_ip, argv[i + 1]);
			server_ip = cserver_ip;
		} else if (strcmp(argv[i], "-p") == 0) {
			server_port = atoi(argv[i + 1]);
		} else if (strcmp(argv[i], "-d") == 0) {
			directory = argv[i + 1];
		}
	}
	cout << "Clients's parameters are:\n\t serverIP: " << server_ip
			<< "\n\t port: " << server_port << "\n\t directory: " << directory
			<< endl;
	/***************************/

	/* open socket etc. */
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	if ((rem = gethostbyname(server_ip.c_str())) == NULL) {
		herror("gethostbyname");
		exit(1);
	}
	server.sin_family = AF_INET;
	memcpy(&server.sin_addr, rem->h_addr, rem->h_length);
	server.sin_port = htons(server_port);
	if (connect(sock, serverptr, sizeof(server)) < 0) {
		perror("connect");
		exit(EXIT_FAILURE);
	}
	cout << "Connecting to " << server_ip << " on port " << server_port << endl;
	/**********************/

	/* request directory */
	char *buffer = new char[directory.size() + 1];
	buffer[directory.size()] = 0;
	memcpy(buffer, directory.c_str(), directory.size());
	uint32_t size = (htonl((uint32_t) strlen(buffer)));
	if (write(sock, &size, sizeof(size)) == -1) {
		perror("write");
		exit(EXIT_FAILURE);
	}
	if (write_all(sock, buffer, strlen(buffer)) == -1) {
		perror("write");
		exit(EXIT_FAILURE);
	}
	/*********************/

	/* receive answer */
	if(directory=="/*end*/")
		return 0;
	int iMode = 0;
	ioctl(sock, FIONBIO, &iMode);	// socket mode: blocking
	string path = "";
	while (1) {

		if ((path = read_all_dir(sock)).empty()) {
			// error
			return -1;
		}

		if (path == "/*end*/")
			break;	// exit

		string file = path.substr(path.find_last_of("/") + 1);	//get name of file
		make_path(path, "", file);	// create the path

		/* update the created files with received data */
		int output;
		string ch = "";
		if ((output = open(path.c_str(), O_WRONLY)) == -1) {
			perror(path.c_str());
		}
		while (ch != "/*end*/") {
			ch.erase();
			ch.clear();
			ch = read_all_dir(sock);
			if (ch == "/*end*/") {
				// eof
				close(output);
				break;
			}
			write(output, ch.c_str(), strlen(ch.c_str()));
		}
		/***********************************************/
		path.erase();
		path.clear();
	}
	/******************/
	close(sock);
}
