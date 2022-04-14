#pragma once

#include "ram.hpp"
#include <iostream>
#include <fstream>
#include <string>

template <typename Out>
void split(const std::string &s, char delim, Out result);

void load(char* path, RAM& ram);