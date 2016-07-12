#pragma once
#include <stdio.h>
#include <cstdio>

#include <tchar.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>

#ifndef F_C_H
#define F_C_H

using namespace std;
using namespace std::tr2::sys;

path getPath(recursive_directory_iterator);

//функция вывода путей сразу с двух папок
void printPath(path);

wstring check_string(wstring);

path check_str_path(path);

int dir_level(wstring);

int dir_level(path);

wstring get_path_str();

path get_path();

path copyPath(path, path);

void copyFolder();
#endif