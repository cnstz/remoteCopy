#ifndef THREAD_LIST_HPP_
#define THREAD_LIST_HPP_

#include <pthread.h>

struct thread_list_node {
	pthread_t *thread;
	thread_list_node *next;

	thread_list_node(pthread_t*, thread_list_node*);
	~thread_list_node();
};

class thread_list {
private:
	thread_list_node *_head;
	int _size;
public:
	thread_list();
	~thread_list();

	void push_back(pthread_t*);

	pthread_t* pop_front();

	int get_size();

};

#endif /* THREAD_LIST_HPP_ */
