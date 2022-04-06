#include "../inc/loader.hpp"

#include "../inc/ram.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}

void load(char* path, RAM& ram) 
{
    using namespace std;
    ifstream myfile;
    myfile.open(path);
    string line;
    if (myfile.is_open()) 
    {
        while (getline(myfile, line)) {
            vector<string> words;
            split(line, ' ', back_inserter(words));   
            for (string w : words) {
                unsigned int data;   
                std::stringstream ss;
                ss << std::hex << w;
                ss >> data;
                ram.Insert(data);
            }  
        }
        myfile.close();
    } else 
        cout << "Unable to open file" << endl;
    return;
}
