#ifndef THREAD_INFO_LIST_HPP_
#define THREAD_INFO_LIST_HPP_

#include <string>

struct thread_info {
	std::string info;
};

struct thread_info_list_node {
	thread_info *info;
	thread_info_list_node *next;

	thread_info_list_node(thread_info*, thread_info_list_node*);
	~thread_info_list_node();
};

class thread_info_list {
private:
	thread_info_list_node *_head;
	int _size;
public:
	thread_info_list();
	~thread_info_list();

	void push_back(thread_info*);

	thread_info* pop_front();

	thread_info* get_back();

	int get_size();

};

#endif /* THREAD_INFO_LIST_HPP_ */
