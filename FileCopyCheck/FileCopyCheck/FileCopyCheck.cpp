// FileCopyCheck.cpp: ���������� ����� ����� ��� ����������� ����������.
//
//#define _CRT_SECURE_NO_WARNINGS
//#define INITGUID
//#define DBINITCONSTANTS


//#import "msado15.dll" 
#include "stdafx.h"
#include <stdio.h>
#include <windows.h>

#import "c:\Program Files (x86)\Common Files\System\ADO\msado20.tlb"  rename ("EOF","adoEOF") no_namespace 
//const GUID CLSID_MSDASQL = { 0xC8B522CBL,0x5CF3,0x11CE,{ 0xAD,0xE5,0x00,0xAA,0x00,0x44,0x77,0x3D } };

// OLE DB - ODBC ����������




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
	int start = filepath.wstring().find_last_of(folderName, filepath.wstring().length()) + 1;
	wstring addpath = filepath.wstring().substr(start);
	path cpPath(destpath.wstring() + addpath);
	return cpPath;
}

void copyFolder() {
	time_t t = time(NULL);
	tm* aTm = localtime(&t);
	std::cout << aTm->tm_hour << ":" << aTm->tm_min << ":" << aTm->tm_sec << " " << aTm->tm_mday << "/" << aTm->tm_mon + 1 << "/" << 1900 + aTm->tm_year << endl;
	wstring source; path sourceFolder;
	wstring dest; path destinationFolder;
	int rcount = 0;
	std::cout << "������� ���� �������� �����." << endl;
	while (!exists(sourceFolder)) {
		sourceFolder = get_path();
		if (exists(sourceFolder)) {
			continue;
		}
		std::cout << "������ ���� �� ����������. ������� �����." << endl;
	}
	std::cout << "������� ���� �������� �����." << endl;
	while (!exists(destinationFolder.parent_path()) || (sourceFolder.stem() != destinationFolder.stem()) || (destinationFolder == sourceFolder)) {
		destinationFolder = get_path();
		bool f = sourceFolder.stem() != destinationFolder.stem();
		if (sourceFolder.stem() != destinationFolder.stem()) {
			std::cout << "�������� ����� �� ���������" << endl;
			continue;
		}
		if (sourceFolder == destinationFolder) {
			std::cout << "��� �� �� ����� �����, �����" << endl;
			continue;
		}
		if (exists(destinationFolder.parent_path())) {
			continue;
		}
		std::cout << "������������� ����� �� ����������. ������� ����� ����." << endl;
	}





	ofstream log("log.txt", ios_base::trunc);
	log << aTm->tm_hour << ":" << aTm->tm_min << ":" << aTm->tm_sec << " " << aTm->tm_mday << "/" << aTm->tm_mon + 1 << "/" << 1900 + aTm->tm_year << endl;
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

		if (is_directory(*it_source)) {
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
		++it_source;
	}

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
	log.close();
}

void show_error(unsigned int handletype, const SQLHANDLE &handle)
{
	SQLWCHAR sqlstate[1024];
	SQLWCHAR message[1024];
	if (SQL_SUCCESS == SQLGetDiagRec(handletype, handle, 1, sqlstate, NULL, message, 1024, NULL))
		wcout << "Message: " << message << "\nSQLSTATE: " << sqlstate << endl;
}


int main(int argc, char* argv[])
{
	setlocale(LC_CTYPE, "rus");
	
	HRESULT hr = S_OK;
	try
	{
		CoInitialize(NULL);
		// Define string variables.
		_bstr_t strCnn("Provider=SQLOLEDB.1;Persist Security Info=True; User ID=sa;Password=1;Initial Catalog=web-adku;Data Source=srv-tm-ias;");
		
		_RecordsetPtr pRstAuthors = NULL;

		// Call Create instance to instantiate the Record set
		hr = pRstAuthors.CreateInstance(__uuidof(Recordset));

		if (FAILED(hr))
		{
			printf("Failed creating record set instance\n");
			return 0;
		}

		//Open the Record set for getting records from Author table
		pRstAuthors->Open("SELECT connectionstring from branches", strCnn, adOpenStatic, adLockReadOnly, adCmdText);

		//Declare a variable of type _bstr_t
		_bstr_t valField1;
		int valField2;

		pRstAuthors->MoveFirst();

		//Loop through the Record set
		if (!pRstAuthors->adoEOF)
		{
			while (!pRstAuthors->adoEOF)
			{
				valField1 = pRstAuthors->Fields->GetItem("connectionstring")->Value;
				//valField2 = pRstAuthors->Fields->GetItem("Author_ID")->Value.intVal;
				cout << ((LPCSTR)valField1) << endl;
				pRstAuthors->MoveNext();
			}
		}

	}
	catch (_com_error &ce)
	{
		cout << ce.Description() << endl;
		//printf("Error:%s\n", ce.Description());
	  //printf(&ce.Error);
	}

	CoUninitialize();
	
	

	std::system("pause");
    return 0;
} 