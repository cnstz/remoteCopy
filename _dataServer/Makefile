all: main.o file_queue.o server_helper.o socket_id_list.o thread_info_list.o thread_list.o socket_files_info_list.o
	g++ -pthread -o ../dataServer main.o file_queue.o server_helper.o socket_id_list.o thread_info_list.o thread_list.o socket_files_info_list.o


main.o: main.cpp
	g++ -c main.cpp


file_queue.o: file_queue.cpp file_queue.hpp
	g++ -c file_queue.cpp


server_helper.o: server_helper.cpp server_helper.hpp
	g++ -c server_helper.cpp


socket_id_list.o: socket_id_list.cpp socket_id_list.hpp
	g++ -c socket_id_list.cpp


thread_info_list.o: thread_info_list.cpp thread_info_list.hpp
	g++ -c thread_info_list.cpp


thread_list.o: thread_list.cpp thread_list.hpp
	g++ -c thread_list.cpp


socket_files_info_list.o: socket_files_info_list.cpp socket_files_info_list.hpp
	g++ -c socket_files_info_list.cpp


clean:
	rm -rf *.o

