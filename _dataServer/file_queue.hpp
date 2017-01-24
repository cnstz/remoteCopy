#ifndef FILE_QUEUE_HPP_
#define FILE_QUEUE_HPP_

#include <string>

struct path_info{
	std::string path;
	int socket;

	path_info(std::string, int);
	~path_info();

};

struct file_queue_node {
	std::string path;
	int socket;

	file_queue_node *next;

	file_queue_node(std::string, int, file_queue_node*);
	~file_queue_node();
};

class file_queue {
private:
	file_queue_node *_head;
	int _size;
	int _max_queue_size;
public:
	file_queue(int);
	~file_queue();

	void push_back(std::string, int);

	path_info* pop_front();

	void print_queue();

	int get_size();

	int get_max_size();

};

#endif /* FILE_QUEUE_HPP_ */
