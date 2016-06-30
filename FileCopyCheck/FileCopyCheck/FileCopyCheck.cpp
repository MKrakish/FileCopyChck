// FileCopyCheck.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"

using namespace std;
using namespace std::tr2::sys;

//функция получения пути через итератор
path getPath(recursive_directory_iterator it) {
	const directory_entry &entry = *it;
	const path &p = entry.path();
	return p;
}

void printPath(recursive_directory_iterator it1, recursive_directory_iterator it2, int c1, int c2)
{
	if (!is_directory(*it1) && !is_directory(*it2)) {
		std::cout << *it1 << " " << c1 << "	" << c2 << " " << *it2 << '\n';
	}
}

void printPath(recursive_directory_iterator it, bool dest, int c)
{
	if (!is_directory(*it) && dest) {
		std::cout <<  "		" << "	" << c << " " << *it << '\n';
	}
	else {
		if (!is_directory(*it)) {
			std::cout << *it << " " << c << '	' << '\n';
		}
	}
}

int main(int argc, char* argv[])
{
	path sourceFolder(L"D:\\SUZUN");
	path destinationFolder(L"D:\\for project\\SUZUN");

	
	//wcout << firstFolderpath.parent_path();
	recursive_directory_iterator it_source(sourceFolder);
	recursive_directory_iterator it_dest(destinationFolder);
	const recursive_directory_iterator it_end;

	//cout << sourceFolder.stem() << endl;
	//cout << destinationFolder.stem() << endl;
	int s = 0, d = 0;
	while (it_source != it_end || it_dest != it_end)
	{
		if (is_directory(*it_source)) {
			++it_source; ++s;
			continue;
		}
		if (is_directory(*it_dest)) {
			++it_dest; ++d;
			continue;
		}
		
		if (it_source != it_end && it_dest == it_end) {
			printPath(it_source, 0,s);
			//std::cout << *it_source << "	" << '\n';
			++it_source; ++s;
		
		}
		if (it_source == it_end && it_dest != it_end) {
			printPath(it_dest, 1,d);
			//std::cout << "		" << "	" << *it_dest << '\n';
			++it_dest; ++d;
			
		}
		if (it_source != it_end && it_dest != it_end) {
			//const directory_entry &entrys = *it_source;
			//const path &ps = entrys.path();
			path ps = getPath(it_source);
			path pd = getPath(it_dest);
			if (ps.filename() == pd.filename())
			{
				printPath(it_source, it_dest, s, d);
				//std::cout << *it_source << '	' << *it_dest << '\n';
				++it_source; ++s;
				++it_dest; ++d;
			}
			else {
				if (ps.filename() > pd.filename()) {
					if (ps.filename() != pd.filename())
					{
						printPath(it_dest, 1, d);
						//std::cout << "		" << "	" << *it_dest << '\n';
						++it_dest; ++d;
										}
				}
				else {
					if (ps.filename() != pd.filename())
					{
						printPath(it_source, 0, s);
						//std::cout << *it_source << '	' << '\n';
						++it_source; ++s;
					}
				}
				
			}
			//cout << *it_source << '	' << *it_dest << '\n';
			
		}
			//++it_source;
			//++it_dest;
	}
	

	//cout << *it_dest;
	
	/*for (auto& p : recursive_directory_iterator(sourceFolder)) 
		if (is_directory(p)) 
		std::cout << "(DIR) " << p << '\n';

	for (auto& p : recursive_directory_iterator(sourceFolder))
		if (!is_directory(p))
			std::cout << "      " << p << '\n';
		
	cout << endl;
	for (auto& p : recursive_directory_iterator(destinationFolder))
		if (is_directory(p))
			std::cout << "(DIR) " << p  << '\n';

	for (auto& p : recursive_directory_iterator(destinationFolder))
		if (!is_directory(p))
			std::cout << "      " << p << '\n';*/
	
	
	

	//сравнение с последующей перезаписью
	if (last_write_time("D:\\SUZUN\\lol.txt") > last_write_time("D:\\for project\\SUZUN\\lol.txt")) {
		std::cout << "source";
		copy_file("D:\\SUZUN\\lol.txt", "D:\\for project\\SUZUN\\lol.txt", copy_options::overwrite_existing);
	}
	else 
	{
		std::cout << "destination";
	}


	std::cout << endl;
	path first("D:\\SUZUN\\lol.txt");
	path second("D:\\for project\\SUZUN\\lol.txt");
	std::cout << first.filename() << ' ' << second.filename() << endl;
	std::system("pause");
    return 0;
} 