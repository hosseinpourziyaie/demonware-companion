#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>     // std::cout
#include <vector>       // std::vector
#include <fstream>      // std::ifstream
#include <sstream>      // std::stringstream
#include <iomanip>      // std::put_time
#include <ctime>        // std::time_t, struct std::tm, std::localtime
#include <io.h>         // posix::access

std::string convert_unix_time(uint32_t timestamp);
bool is_file_exists(const std::string& filename);
std::vector<unsigned char> read_binary_file(const std::string filename);
