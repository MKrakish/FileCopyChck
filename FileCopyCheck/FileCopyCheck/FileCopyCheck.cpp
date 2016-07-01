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
void printPath(recursive_directory_iterator it1, recursive_directory_iterator it2)
{
	if (!is_directory(*it1) && !is_directory(*it2)) {
		std::cout << *it1 <<   "		" << *it2 << '\n';
	}
}
//������� ������ ���� � ����� �����
void printPath(recursive_directory_iterator it, bool dest)
{
	if (!is_directory(*it) && dest) { // ��������� ����� �� �������� ��� ���������, ��� ��� ��� ���������� ������ �����
		std::cout <<  "		" << "		" << *it << '\n';
	}
	else {
		if (!is_directory(*it)) {
			std::cout << *it << "		" <<  '\n';
		}
	}
}

path copyPath(path filepath, path destpath) {
	string folderName = destpath.parent_path().stem().string();
	string addpath = filepath.string().substr(folderName.length() + filepath.string().find(folderName, 0) - 2);
	path cpPath (destpath.string() +addpath);
	return cpPath;
}

int main(int argc, char* argv[])
{
	ofstream log("log.txt");	
	setlocale(LC_CTYPE, "rus");
	path sourceFolder(L"D:\\SUZUN"); // �������� �����
	path destinationFolder(L"D:\\for project\\SUZUN"); // ��������

	
	//��������� ��������� 
	recursive_directory_iterator it_source(sourceFolder);
	recursive_directory_iterator it_dest(destinationFolder);
	const recursive_directory_iterator it_end;

	
	
	while (it_source != it_end || it_dest != it_end)
	{
		
		if (it_source != it_end && it_dest == it_end) {							// ������, �������� ������ ��� ����� � ����� �������� �����
			printPath(it_source, 0);
			path ps = getPath(it_source);
			++it_source;
			copy(ps, copyPath(ps, destinationFolder).parent_path());
			log << copyPath(ps, destinationFolder) << "		" << "��� ����������" << endl;
		
		}
		if (it_source == it_end && it_dest != it_end) {							// ������, �������� ������ ��� ����� � ����� �������� �����
			printPath(it_dest, 1);
			++it_dest; 
			
		}
		if (it_source != it_end && it_dest != it_end) {							// ������ ����� ��� ��������� �� ������ ��� ����� �����
			path ps = getPath(it_source);
			path pd = getPath(it_dest);											// ��������� ���� �� ���������
			if (is_directory(*it_source)) {										// ����� ��� �� ����������
				++it_source; 
				continue;
			}
			if (is_directory(*it_dest)) {										// ����� ��� �� ����������
				++it_dest; 
				continue;  
			}

			if (ps.parent_path().stem() == pd.parent_path().stem()) {			//�������� �� ������������ ������
				if (ps.filename() == pd.filename())								// �����
				{

					printPath(it_source, it_dest);
					if (last_write_time(ps) > last_write_time(pd)) {
						copy_file(ps, pd, copy_options::overwrite_existing);
						log << pd << "		" << "��� �������" << endl;
					}
					++it_source;
					++it_dest;
				}
				else {
					if (ps.filename() > pd.filename()) {
						if (ps.filename() != pd.filename())
						{
							printPath(it_dest, 1);
							++it_dest;
						}
					}
					else {
						if (ps.filename() != pd.filename())
						{
							printPath(it_source, 0);
							path ps = getPath(it_source);
							copy(ps, copyPath(ps, destinationFolder).parent_path());
							log << copyPath(ps, destinationFolder) << "		" << "��� ����������"<< endl;
							++it_source;
						}
					}
				}
			}
			else {																// ������ ����� ������������ ����� � ����������� ������� ����� � ����� ������
				if (ps.parent_path().stem() > pd.parent_path().stem()) {
					if (ps.parent_path().stem() != pd.parent_path().stem())
					{
						printPath(it_dest, 1);
						++it_dest;
					}
				}
				else {
					if (ps.parent_path().stem() != pd.parent_path().stem())
					{
						printPath(it_source, 0);
						path ps = getPath(it_source);
						path cpdebug = copyPath(ps, destinationFolder);
						copy(ps, copyPath(ps, destinationFolder).parent_path());
						log << copyPath(ps, destinationFolder) << "		" << "��� ����������" << endl;
						++it_source;
					}
				}

			}
					
		}
	}
	
	log.close();
	
	
	

	//��������� � ����������� �����������
	


	std::cout << endl;
	path first("D:\\SUZUN\\Data\\2.txt");
	path second("D:\\for project\\SUZUN");
	string folderName =second.parent_path().stem().string();
	std::cout << first.string().find(folderName, 0)<< endl;
	std::cout << first.string().substr(folderName.length() + 3) << endl;
	std::cout << second.string() + first.string().substr(folderName.length() + 3 - 1)<< endl;
	std::system("pause");
    return 0;
} 