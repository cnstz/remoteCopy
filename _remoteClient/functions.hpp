#ifndef FUNCTIONS_HPP_
#define FUNCTIONS_HPP_

#include <string>

int write_all(int, char*, size_t); // writes char* of size_t to int descriptor

std::string read_all_dir(int);	// reads data from int descriptor

void make_path(std::string, std::string, std::string);	// creates given path and file

#endif /* FUNCTIONS_HPP_ */
