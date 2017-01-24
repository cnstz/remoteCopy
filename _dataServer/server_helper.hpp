#ifndef SERVER_HELPER_HPP_
#define SERVER_HELPER_HPP_

#include <string>
#include "file_queue.hpp"
#include "thread_list.hpp"
#include <dirent.h>
#include <cstdio>
#include <cstdlib>


int write_all(int, char*, size_t);
std::string read_all_dir(int, pid_t &client_pid);

DIR* open_directory(std::string);
std::string get_directory(int);
bool add_dir(std::string, int);
void count_files(std::string, int);
void* scan_directory(void*);

void *worker(void*);

class server_helper {
public:
	server_helper(int, int);

	~server_helper();

	void destroy_threads();

	bool StartThread(int);

};

#endif /* SERVER_HELPER_HPP_ */
