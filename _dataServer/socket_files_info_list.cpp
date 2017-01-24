#include "socket_files_info_list.hpp"
#include <iostream>

using namespace std;

socket_files_info_node::socket_files_info_node(int _socket,
		socket_files_info_node *n) {
	socket_id = _socket;
	number_of_files = 0;
	files_read = 0;
	next = n;
}

socket_files_info_node::~socket_files_info_node() {
}

socket_files_info_list::socket_files_info_list() :
		_head(NULL), _size(0) {
}

socket_files_info_list::~socket_files_info_list() {
	socket_files_info_node* current = _head;
	while (current != NULL) {
		socket_files_info_node* next = current->next;
		//delete current->data;
		delete current;
		current = next;
	}
	_head = NULL;
}

void socket_files_info_list::update_add_new(int socket) {
	if (_head != NULL) {
		socket_files_info_node *temp = _head;
		if (_head->socket_id == socket) {
			_head->number_of_files++;
			return;
		} else {
			while (temp->next != NULL) {
				temp = temp->next;
				if (temp->socket_id == socket) {
					break;
				}
			}
			if (temp->socket_id == socket) {
				temp->number_of_files++;
				return;
			}
		}
	}
	if (_head == NULL) {
		_head = new socket_files_info_node(socket, NULL);
		_head->number_of_files++;
	} else {
		socket_files_info_node *temp = _head;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = new socket_files_info_node(socket, NULL);
		temp->next->number_of_files++;
	}
	_size++;
}

void socket_files_info_list::update_add_done(int socket) {
	if (_head != NULL) {
		socket_files_info_node *temp = _head;
		if (_head->socket_id == socket) {
			_head->files_read++;
			return;
		} else {
			while (temp->next != NULL) {
				temp = temp->next;
				if (temp->socket_id == socket) {
					break;
				}
			}
			if (temp->socket_id == socket) {
				temp->files_read++;
				return;
			}
		}
	}
}

bool socket_files_info_list::done(int socket) {
	if (_head != NULL) {
		socket_files_info_node *temp = _head;
		if (_head->socket_id == socket) {
			if (_head->files_read == _head->number_of_files)
				return true;
		} else {
			while (temp->next != NULL) {
				temp = temp->next;
				if (temp->socket_id == socket) {
					break;
				}
			}
			if (temp->socket_id == socket) {
				if (temp->files_read == temp->number_of_files)
					return true;

			}
		}
	}
	return false;
}

void socket_files_info_list::pop(int socket) {
	if (_head != NULL) {
		socket_files_info_node *temp = _head;
		if (_head->socket_id == socket) {
			_head = _head->next;
			delete temp;
			_size--;
		} else {
			socket_files_info_node *temp2 = _head;
			while (temp->next != NULL) {
				temp = temp->next;
				if (temp->socket_id == socket) {
					break;
				}
				temp2 = temp2->next;
			}
			if (temp->socket_id != socket)
				return;
			temp2->next = temp->next;
			delete temp;
			_size--;
		}
	}
}

int socket_files_info_list::get_size() {
	return _size;
}

