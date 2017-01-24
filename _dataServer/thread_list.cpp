#include "thread_list.hpp"
#include <iostream>

using namespace std;

thread_list_node::thread_list_node(pthread_t *_thread,
		thread_list_node *n) {
	thread = _thread;
	next = n;
}

thread_list_node::~thread_list_node() {
}

thread_list::thread_list() :
		_head(NULL), _size(0){
}

thread_list::~thread_list() {
	thread_list_node* current = _head;
	while (current != NULL) {
		thread_list_node* next = current->next;
		delete current->thread;
		delete current;
		current = next;
	}
	_head = NULL;
}

void thread_list::push_back(pthread_t *argument) {
	if (_head == NULL) {
		_head = new thread_list_node(argument, NULL);
	} else {
		thread_list_node *temp = _head;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = new thread_list_node(argument, NULL);
	}
	_size++;
}

pthread_t* thread_list::pop_front() {
	if (_head != NULL) {
		thread_list_node *temp = _head;
		_head = _head->next;
		pthread_t* arg = temp->thread;
		delete temp;
		_size--;
		return arg;
	}
	return NULL;
}

int thread_list::get_size() {
	return _size;
}
