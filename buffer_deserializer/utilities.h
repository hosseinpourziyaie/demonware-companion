#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>     // std::cout
#include <vector>       // std::vector
#include <fstream>      // std::ifstream
#include <sstream>      // std::stringstream
#include <iomanip>      // std::put_time
#include <ctime>        // std::time_t, struct std::tm, std::localtime
#include <io.h>         // posix::access
#include <direct.h>     // _mkdir

std::string convert_unix_time(uint32_t timestamp);
bool is_file_exists(const std::string& file_path);
bool create_directory_if_not_exists(std::string directory);
std::string get_file_name_from_path(std::string path);
std::vector<unsigned char> read_binary_file(const std::string file_path);
bool write_binary_file(std::vector<unsigned char> buffer, std::string file_path);
