// FileCopyCheck.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"





int main(int argc, char* argv[])
{
	path firstFolderpath(L"D:\\SUZUN\\");
	path secondFolderpath(L"D:\\for project\\SUZUN");

	
	//wcout << firstFolderpath.parent_path();
	
	for (auto& p : directory_iterator(firstFolderpath)) 
		if (is_directory(p)) 
		std::cout << p << " (DIR)" << '\n';

	for (auto& p : directory_iterator(firstFolderpath)) 
		if (!is_directory(p))
			std::cout << p << '\n';
		
	cout << endl;
	for (auto& p : directory_iterator(firstFolderpath))
		if (is_directory(p))
			std::cout << p << " (DIR)" << '\n';

	for (auto& p : directory_iterator(firstFolderpath))
		if (!is_directory(p))
			std::cout << p << '\n';
	
	cout << endl;
	system("pause");
    return 0;
} 