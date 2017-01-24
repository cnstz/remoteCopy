#ifndef SOCKET_ID_LIST_HPP_
#define SOCKET_ID_LIST_HPP_

struct socket_id_list_node {
	int socket_id;
	socket_id_list_node *next;

	socket_id_list_node(int, socket_id_list_node*);
	~socket_id_list_node();
};

class socket_id_list {
private:
	socket_id_list_node *_head;
	int _size;
public:
	socket_id_list();
	~socket_id_list();

	void push_back(int);

	void pop(int);

	bool exists(int);

	int get_size();

};

#endif /* SOCKET_ID_LIST_HPP_ */
