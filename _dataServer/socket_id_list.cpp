#include "socket_id_list.hpp"
#include <iostream>

using namespace std;

socket_id_list_node::socket_id_list_node(int _socket, socket_id_list_node *n) {
	socket_id = _socket;
	next = n;
}

socket_id_list_node::~socket_id_list_node() {
}

socket_id_list::socket_id_list() :
		_head(NULL), _size(0) {
}

socket_id_list::~socket_id_list() {
	socket_id_list_node* current = _head;
	while (current != NULL) {
		socket_id_list_node* next = current->next;
		//delete current->data;
		delete current;
		current = next;
	}
	_head = NULL;
}

void socket_id_list::push_back(int socket) {
	if (_head == NULL) {
		_head = new socket_id_list_node(socket, NULL);
	} else {
		socket_id_list_node *temp = _head;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = new socket_id_list_node(socket, NULL);
	}
	_size++;
}

void socket_id_list::pop(int socket) {
	if (_head != NULL) {
		socket_id_list_node *temp = _head;
		if (_head->socket_id == socket) {
			_head = _head->next;
			delete temp;
			_size--;
		} else {
			socket_id_list_node *temp2 = _head;
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

bool socket_id_list::exists(int socket) {
	if (_head) {
		socket_id_list_node *temp = _head;
		while (temp->next != NULL) {
			if (temp->socket_id == socket) {
				return true;
			}
			temp = temp->next;
		}
	}
	return false;
}

int socket_id_list::get_size() {
	return _size;
}

