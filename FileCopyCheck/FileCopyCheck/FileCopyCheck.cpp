// FileCopyCheck.cpp: ���������� ����� ����� ��� ����������� ����������.
//
//#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"


using namespace std;
using namespace std::tr2::sys;

//������� ��������� ���� ����� ��������
path getPath(recursive_directory_iterator it) {
	const directory_entry &entry = *it;
	const path &p = entry.path();
	return p;
}

//������� ������ ����� ����� � ���� �����
void printPath(path p1) {
	std::cout << p1 << '\n';
}
wstring check_string(wstring str) {
	if (str[str.length() - 1] == '\\') {
		str.erase(str.end() - 1, str.end());
	}
	return str;
}

path chech_str_path(path pth) {
	wstring str = pth.wstring();
	str = check_string(str);
	pth = path(str);
	return pth;
}

int dir_level(wstring dir) {
	int count = 0;
	for (int i = 0; i < dir.length(); i++) {
		if (dir[i] == '\\') {
			count++;
		}
	}
	return count;
}
int dir_level(path dir) {
	int count = 0; wstring dirstr;
	dirstr = dir.wstring();
	count = dir_level(dirstr);
	return count;
}

wstring get_path_str() {
	wstring dir_string; bool isPath = 0;
	while (!isPath) {
		getline(std::wcin, dir_string);
		path dir(dir_string);
		if (dir.parent_path() == "\0") {
			std::cout << "�������� ����� �� �������� ����. ���������� �����." << endl;
		}
		else {
			isPath = 1;
		}
	}
	dir_string = check_string(dir_string);
	return dir_string;
}

path get_path() {
	wstring dir_string = get_path_str();
	path dir(dir_string);
	return dir;
}
	


path copyPath(path filepath, path destpath) {
	wstring folderName = destpath.stem().wstring();
	//std::wcout << folderName << endl;
	int start = filepath.wstring().find_last_of(folderName, filepath.wstring().length())+1;
	//std::wcout << start << endl;
	wstring addpath = filepath.wstring().substr(start); 
	//std::wcout << addpath << endl;																							
	path cpPath (destpath.wstring() + addpath);
	return cpPath;
}

int main(int argc, char* argv[])
{
	setlocale(LC_CTYPE, "rus");
	time_t t = time(NULL);
	tm* aTm = localtime(&t);
	std::cout << aTm->tm_hour << ":" << aTm->tm_min << ":" << aTm->tm_sec << " " << aTm->tm_mday << "/" << aTm->tm_mon + 1 << "/" << 1900 + aTm->tm_year << endl;
	wstring source; path sourceFolder;
	wstring dest; path destinationFolder;
	int rcount = 0;
	//wstring test;
	//getline(std::wcin, source); //wcin >> source;
	//dest = L"D:\\for project\\SUZUN\\Data\\card\\SUZUN"; //\\Data\\SUZUN
	//source = L"D:\\SUZUN\\Data\\SUZUN\\" ;//\\Data\\card
	//path psource(source);
	//path pdest(dest);
	//std::cout << copyPath(path(source), path(dest)) << endl;
	//std::cout << source.substr(3,6) << endl; 
	//std::cout << source.find_last_of(L"SUZUN", source.length()) << endl;
	//std::cout << psource.string().find(psource.stem().string(), 0) << endl;
	//std::cout << psource.wstring().substr(pdest.stem().string().length() + psource.wstring().find(folderName, 0)) << endl;
	//getline(std::wcin, dest); //wcin >> dest;
	//source = check_string(source);
	//dest = check_string(dest);
	std::cout << "������� ���� �������� �����." << endl;
	while (!exists(sourceFolder)) {
		sourceFolder = get_path();
		if (exists(sourceFolder)) {
			continue;
		}
		std::cout << "������ ���� �� ����������. ������� �����." << endl;
	}
	//std::cout <<
	std::cout << "������� ���� �������� �����." << endl;
	while (!exists(destinationFolder.parent_path())) {
		destinationFolder = get_path();
		if (exists(destinationFolder.parent_path())) {
			continue;
		}
		std::cout << "������������� ����� �� ����������. ������� ����� ����." << endl;
	}
	
	
	

	
	ofstream log("log.txt", ios_base::trunc);
	log << aTm->tm_hour << ":" << aTm->tm_min << ":" << aTm->tm_sec << " " << aTm->tm_mday << "/" << aTm->tm_mon + 1 << "/" << 1900 + aTm->tm_year << endl;
	//(source); // �������� �����
	//path destinationFolder(dest); // ��������

	//std::cout << sourceFolder.stem() << " " << destinationFolder.stem() << endl;
	//std::cout << copyPath(destinationFolder, sourceFolder) << endl;
	//copyPath(sourceFolder, destinationFolder);

	//��������� ��������
	recursive_directory_iterator it_source(sourceFolder);
	const recursive_directory_iterator it_end;

	
	if (!exists(destinationFolder)) {
		create_directory(destinationFolder);
	}


	std::cout << "��� \"���-�����\" 2016." << endl;
	std::cout << "����������� ��������" << endl;
	int count = 0;
	while (it_source != it_end)
	{
		path ps = getPath(it_source);
		path newpath = copyPath(ps, destinationFolder);

		if (is_directory(*it_source)) {										// ����� ��� �� ����������
			++it_source;
			continue;
	}
				
		if (!exists(newpath))
		{
			create_directories(newpath.parent_path());
				copy(ps, newpath); count++;
			log << newpath << "\t" << "��� ����������" << endl;
		}
		else {
			if (last_write_time(ps) > last_write_time(newpath)) {
				copy(ps, newpath, copy_options::overwrite_existing); count++;
				log << newpath << "\t" << "��� �������" << endl;
			}
		}
		//printPath(newpath);
		++it_source;
}
	
	//std::cout << count<< endl;
	if (count == 0) {
		std::cout << "����� � ���������� �� ���������, ���� �� ����������� ������ �����-��������" << endl;
		log << "����� � ���������� �� ���������, ���� �� ����������� ������ �����-��������" << endl;
	}
	else {
		std::cout << "����������� ���������. log.txt - ������ � ������������ ������." << endl;
		std::cout << "����������� " << count << " ������(�)" << endl;
		log << "����������� ���������. log.txt - ������ � ������������ ������." << endl;
		log << "����������� " << count << " ������(�)" << endl;
	}
	//std::wcout << source << endl;
	//std::wcout << dest << endl;
	log.close();
	std::system("pause");
    return 0;
} 