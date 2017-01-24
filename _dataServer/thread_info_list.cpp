#include "thread_info_list.hpp"
#include <iostream>

using namespace std;

thread_info_list_node::thread_info_list_node(thread_info *_info,
		thread_info_list_node *n) {
	info = _info;
	next = n;
}

thread_info_list_node::~thread_info_list_node() {
}

thread_info_list::thread_info_list() :
		_head(NULL), _size(0) {
}

thread_info_list::~thread_info_list() {
	thread_info_list_node* current = _head;
	while (current != NULL) {
		thread_info_list_node* next = current->next;
		//delete current->thread;
		delete current;
		current = next;
	}
	_head = NULL;
}

void thread_info_list::push_back(thread_info *argument) {
	if (_head == NULL) {
		_head = new thread_info_list_node(argument, NULL);
	} else {
		thread_info_list_node *temp = _head;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = new thread_info_list_node(argument, NULL);
	}
	_size++;
}

thread_info* thread_info_list::pop_front() {
	if (_head != NULL) {
		thread_info_list_node *temp = _head;
		_head = _head->next;
		thread_info* arg = temp->info;
		delete temp;
		_size--;
		return arg;
	}
	return NULL;
}

thread_info* thread_info_list::get_back() {
	if (_head != NULL) {
		thread_info_list_node *temp = _head;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		return temp->info;
	}
	return NULL;
}

int thread_info_list::get_size() {
	return _size;
}
