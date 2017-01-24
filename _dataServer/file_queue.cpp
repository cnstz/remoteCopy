#include "file_queue.hpp"
#include <iostream>

using namespace std;

path_info::path_info(std::string _path, int _socket) {
	path = _path;
	socket = _socket;
}

path_info::~path_info() {
}

file_queue_node::file_queue_node(string _path, int _socket,
		file_queue_node *n) {
	path = _path;
	socket = _socket;
	next = n;
}

file_queue_node::~file_queue_node() {
}

file_queue::file_queue(int max_queue_size) :
		_head(NULL), _size(0), _max_queue_size(max_queue_size) {
}

file_queue::~file_queue() {
	file_queue_node* current = _head;
	while (current != NULL) {
		file_queue_node* next = current->next;
		//delete current->data;
		delete current;
		current = next;
	}
	_head = NULL;
}

void file_queue::push_back(string argument, int socket) {
	if (_head == NULL) {
		_head = new file_queue_node(argument, socket, NULL);
	} else {
		file_queue_node *temp = _head;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = new file_queue_node(argument, socket, NULL);
	}
	_size++;
}

path_info* file_queue::pop_front() {
	if (_head != NULL) {
		file_queue_node *temp = _head;
		_head = _head->next;
		string arg = temp->path;
		int socket = temp->socket;
		path_info* temp_info = new path_info(arg, socket);
		delete temp;
		_size--;
		return temp_info;
	}
	return NULL;
}

void file_queue::print_queue() {
	file_queue_node *temp = _head;
	cout << temp->path << " ";
	while (temp->next != NULL) {
		temp = temp->next;
		cout << temp->path << endl;

	}
	cout << endl;
}

int file_queue::get_size() {
	return _size;
}

int file_queue::get_max_size() {
	return _max_queue_size;
}
