/*
 * socket_files_info_list.hpp
 *
 *  Created on: Jul 12, 2014
 *      Author: constantine
 */

#ifndef SOCKET_FILES_INFO_LIST_HPP_
#define SOCKET_FILES_INFO_LIST_HPP_

struct socket_files_info_node {
	int socket_id;
	int number_of_files;
	int files_read;
	socket_files_info_node *next;

	socket_files_info_node(int, socket_files_info_node*);
	~socket_files_info_node();
};

class socket_files_info_list {
private:
	socket_files_info_node *_head;
	int _size;
public:
	socket_files_info_list();
	~socket_files_info_list();

	void update_add_new(int);

	void update_add_done(int);

	bool done(int);

	void pop(int);

	int get_size();

};

#endif /* SOCKET_FILES_INFO_LIST_HPP_ */
