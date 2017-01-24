#include "functions.hpp"
#include <cstdio>
#include <cstdlib>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <cstring>
#include <iostream>

using namespace std;

int write_all(int fd, char* buff, size_t size) {
	int sent, n;
	for (sent = 0; (unsigned) sent < size; sent += n) {
		if ((n = write(fd, buff + sent, size - sent)) == -1)
			return -1; /* error */
	}
	return sent;
}

string read_all_dir(int fd) {
	/* get the size of the message*/
	int received, n;
	uint32_t rsize;
	read(fd, &rsize, sizeof(uint32_t));
	/******************************/

	/* read the message */
	size_t size = ntohl(rsize);
	char *buff = (char*) malloc(size);
	for (received = 0; (unsigned) received < size; received += n) {
		if ((n = read(fd, buff + received, size - received)) == -1)
			return ""; /* error */
	}
	/********************/

	string temp = "";
	for (int i = 0; (unsigned) i < size; i++) {
		temp.push_back(buff[i]);
	}
	return temp;
}

void make_path(string path, string folder, string filename) {
	if (path.length() == 0)
		return;
	int i;
	/* string operations */
	for (i = 0; (unsigned) i < path.length(); i++) {
		folder.push_back(path[i]);
		if (path[i] == '/')
			break;
	}
	/*********************/
	/* path creation*/
	if (folder.substr(folder.find_last_of("/") + 1) == filename) {
		// case: file
		int output;
		if ((output = open(folder.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0777))
				== -1) {
			perror(folder.c_str());
			return;
		}
		close(output);
	} else {
		// case: folder
		mkdir(folder.c_str(), 0777);
		if ((unsigned) (i + 1) < path.length()) {
			make_path(path.substr(i + 1), folder, filename);
		}
	}
	/****************/
}

