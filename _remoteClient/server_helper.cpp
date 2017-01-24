#include "server_helper.hpp"
#include <cstdio>
#include <fcntl.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <pthread.h>
#include "thread_info_list.hpp"
#include "socket_id_list.hpp"
#include <stdint.h>
#include <netinet/in.h>
#include <cstring>
#include "socket_files_info_list.hpp"

pthread_mutex_t lock;
pthread_cond_t cond;

using namespace std;

bool pool_status;
int thread_pool_size;
thread_list *thread_pool;
file_queue *execution_queue;
thread_info_list threads_info;
socket_id_list current_sockets;
socket_files_info_list socket_file_info;

int write_all(int fd, char* buff, size_t size) {
	int sent, n;
	for (sent = 0; (unsigned) sent < size; sent += n) {
		if ((n = write(fd, buff + sent, size - sent)) == -1)
			return -1; /* error */
	}
	return sent;
}

string read_all_dir(int fd) {
	int received, n;
	//uint32_t rsize;
	int size;
	while(read(fd, &size, sizeof(int))==0){}
	//size_t size = ntohl(rsize);
	cout<<size<<endl;
	char *buff = (char*) malloc(size);
	for (received = 0; (unsigned) received < size; received += n) {
		if ((n = read(fd, buff + received, size - received)) == -1){perror("FFF");
			return ""; /* error */}
	}


	string temp(buff);
	return temp;
}

DIR* open_directory(string dirname) {
	DIR *dir_ptr;
	if ((dir_ptr = opendir(dirname.c_str())) == NULL)
		return NULL;
	else
		return dir_ptr;
}

string get_directory(int newsock) {
	string directory = "";
	directory = read_all_dir(newsock);
	return directory;
}

bool add_dir(string path, int sock) {
	DIR *dir;
	struct dirent *direntp;
	string file_name;
	struct stat statbuf;
	if ((dir = open_directory(path)) == NULL) {
		perror("open");
		return false;
	}
	while ((direntp = readdir(dir)) != NULL) {
		file_name = direntp->d_name;
		if (file_name == "." || file_name == ".."
				|| file_name[file_name.length() - 1] == '~')
			continue;
		string file_path = path + "/" + file_name;

		if (stat(file_path.c_str(), &statbuf) == -1) {
			perror("file status");
			return false;
		}

		if ((statbuf.st_mode & S_IFMT) == S_IFDIR) {
			// type: directory
			if (!add_dir(file_path, sock)) {
				return false;
			}
		} else {
			// type: file

			cout << "[Thread: " << (unsigned int) pthread_self()
					<< "]: Adding file " << file_path << " to the queue..."
					<< endl;
			while (execution_queue->get_size()
					== execution_queue->get_max_size()) {
			}
			pthread_mutex_lock(&lock);
			execution_queue->push_back(file_path, sock);
			pthread_cond_signal(&cond);
			pthread_mutex_unlock(&lock);
		}
	}
	return true;
}

void count_files(string path, int sock) {
	DIR *dir;
	struct dirent *direntp;
	string file_name;
	struct stat statbuf;
	if ((dir = open_directory(path)) == NULL) {
		perror("open");
	}
	while ((direntp = readdir(dir)) != NULL) {
		file_name = direntp->d_name;
		if (file_name == "." || file_name == ".."
				|| file_name[file_name.length() - 1] == '~')
			continue;
		string file_path = path + "/" + file_name;

		if (stat(file_path.c_str(), &statbuf) == -1) {
			perror("file status");
		}
		if ((statbuf.st_mode & S_IFMT) == S_IFDIR) {
			// Input file is a directory}
			count_files(file_path, sock);
		} else {
			// Input file is a file
			socket_file_info.update_add_new(sock);
		}
	}
}

void* scan_directory(void* newsock) {
	int sock = *(int*) newsock;
	string directory = get_directory(sock);	// get directory name
	if (directory == "/*end*/") {
		close(sock);
		close(pthread_self());
		exit(0);
	}
	cout << "[Thread: " << (unsigned int) pthread_self()
			<< "]: About to scan directory " << directory << endl;
	count_files(directory, sock);	// count files to be copied
	if (!add_dir(directory, sock)) {
		return NULL;
	}
	/********************/
	while (!socket_file_info.done(sock)) {
		// wait until all files are transfered
	}

	/* close everything */
	socket_file_info.pop(sock);
	uint32_t size = (htonl((uint32_t) strlen("/*end*/")));
	if (write(sock, &size, sizeof(size)) == -1) {
		perror("write");
		exit(EXIT_FAILURE);
	}
	char temps[] = "/*end*/";
	if (write_all(sock, temps, strlen(temps)) == -1) {
		perror("write");
		exit(EXIT_FAILURE);
	}
	close(sock);
	close(pthread_self());
	/********************/
	return 0;
}

void* worker(void* arg) {
	while (1) {
		pthread_mutex_lock(&lock);
		while (pool_status && execution_queue->get_size() == 0) {
			// if running wait for job
			pthread_cond_wait(&cond, &lock);
		}
		if (!pool_status) {
			// exiting...
			pthread_mutex_unlock(&lock);
			pthread_exit(NULL);
		}
		/* get job from execution queue */
		path_info *temp = execution_queue->pop_front();
		string file = temp->path;
		int socket = temp->socket;
		cout << "[Thread: " << (unsigned int) pthread_self()
				<< "]: Received task: <" << file << ", " << socket << ">"
				<< endl;
		/********************************/

		while (current_sockets.exists(socket)) {
			// wait if another worker is communicating through that socket
		}
		current_sockets.push_back(socket);	// lock the socket

		/* send path */
		cout << "[Thread: " << (unsigned int) pthread_self()
				<< "]: About to read " << file << endl;
		char *buffer = new char[file.size() + 1];
		memcpy(buffer, file.c_str(), file.size());
		uint32_t size = (htonl((uint32_t) strlen(buffer)));
		if (write(socket, &size, sizeof(size)) == -1) {
			perror("write");
			exit(EXIT_FAILURE);
		}
		if (write_all(socket, buffer, strlen(buffer)) == -1) {
			perror("write");
			exit(EXIT_FAILURE);
		}
		/*************/
		/* send file data */
		int input, ffread;
		if ((input = open(file.c_str(), O_RDONLY)) == -1) {
			perror("open");
		}
		char ch[1];
		while ((ffread = read(input, ch, sizeof(ch))) > 0) {
			uint32_t size2 = (htonl((uint32_t) strlen(ch)));
			if (write(socket, &size2, sizeof(size2)) == -1) {
				perror("write");
				exit(EXIT_FAILURE);
			}
			if (write_all(socket, ch, strlen(ch)) == -1) {
				perror("write");
				exit(EXIT_FAILURE);
			}
		}
		close(input);
		/******************/
		/* send ending sequence */
		uint32_t size3 = (htonl((uint32_t) strlen("/*end*/")));
		if (write(socket, &size3, sizeof(size3)) == -1) {
			perror("write");
			exit(EXIT_FAILURE);
		}
		char temps[] = "/*end*/";
		if (write_all(socket, temps, strlen(temps)) == -1) {
			perror("write");
			exit(EXIT_FAILURE);
		}
		/************************/
		socket_file_info.update_add_done(socket);// another file was transfered
		current_sockets.pop(socket);	// unlock socket
		pthread_mutex_unlock(&lock);
	}
	return NULL;
}

server_helper::server_helper(int max_size, int pool_size) {
	execution_queue = new file_queue(max_size);
	thread_pool_size = pool_size;
	thread_pool = new thread_list();
	pool_status = true;
	/* initialize threads */
	for (int i = 0; i < thread_pool_size; i++) {
		pthread_t *thread_ID(new pthread_t);
		thread_pool->push_back(thread_ID);
		thread_info *temp = new thread_info();
		temp->info = "s";
		threads_info.push_back(temp);
		if (pthread_create(thread_ID, NULL, worker,
				(void *) ((threads_info.get_back()))) < 0) {
			perror("pthread_create");
		}
	}
	/**********************/
}

server_helper::~server_helper() {
	delete execution_queue;
	destroy_threads();
	delete thread_pool;
}

void server_helper::destroy_threads() {
	/* notify threads */
	pthread_mutex_lock(&lock);
	pool_status = false;
	pthread_mutex_unlock(&lock);
	pthread_cond_broadcast(&cond);
	/******************/
	/* close everything */
	void* stat;
	while (thread_pool->get_size() != 0) {
		pthread_join(*(thread_pool->pop_front()), &stat);
	}
	/********************/
}

bool server_helper::StartThread(int newsock) {
	/* create new thread*/
	pthread_mutex_init(&lock, NULL);
	pthread_t thread_id;
	if (pthread_create(&thread_id, NULL, scan_directory, (void*) &newsock) != 0)
		return false;
	return true;
}
