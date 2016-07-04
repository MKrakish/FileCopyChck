// FileCopyCheck.cpp: определяет точку входа для консольного приложения.
//
//#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECUR
#include "stdafx.h"


using namespace std;
using namespace std::tr2::sys;

//функция получения пути через итератор
path getPath(recursive_directory_iterator it) {
	const directory_entry &entry = *it;
	const path &p = entry.path();
	return p;
}

//функция вывода путей сразу с двух папок
void printPath(path p1, path p2) {
	std::cout << p1 << "		" << p2 << '\n';
}
wstring check_string(wstring str) {
	if (str[str.length() - 1] == '\\') {
		str.erase(str.end() - 1, str.end());
	}
	return str;
}
	



path copyPath(path filepath, path destpath) {
	wstring folderName = destpath.parent_path().stem().wstring();
	wstring addpath = filepath.wstring().substr(folderName.length() + filepath.wstring().find(folderName, 0) - 2);
	path cpPath (destpath.wstring() + addpath);
	return cpPath;
}

int main(int argc, char* argv[])
{
	setlocale(LC_CTYPE, "rus");
	time_t t = time(NULL);
	tm* aTm = localtime(&t);
	std::cout << aTm->tm_hour << ":" << aTm->tm_min << ":" << aTm->tm_sec << " " << aTm->tm_mday << "/" << aTm->tm_mon + 1 << "/" << 1900 + aTm->tm_year << endl;
	wstring source;
	wstring dest;
	
	wstring test;
	//getline(std::wcin, source); //wcin >> source;
	dest = L"D:\\SUZUN";
	source = L"D:\\for project\\SUZUN";

	//getline(std::wcin, dest); //wcin >> dest;
	check_string(source);
	check_string(dest);
	bool fault = 0;
	while (!fault) {
		getline(std::wcin, test);
		path test2(test);
		if (test2.parent_path() == "\0") {
			fault = 1;
			std::cout << "Введеный текст не явялется путём" << endl;
		}
	}

	
	ofstream log("log.txt", ios_base::trunc);
	log << aTm->tm_hour << ":" << aTm->tm_min << ":" << aTm->tm_sec << " " << aTm->tm_mday << "/" << aTm->tm_mon + 1 << "/" << 1900 + aTm->tm_year << endl;
	path sourceFolder(source); // исходная папка
	path destinationFolder(dest); // конечная
	
	
	//объявляем итераторы 
	recursive_directory_iterator it_source(sourceFolder);
	const recursive_directory_iterator it_end;

	error_code error;
	if (!exists(destinationFolder)) {
		create_directory(destinationFolder);
	}


	std::cout << "ПКБ \"АСУ-Нефть\" 2016." << endl;
	std::cout << "Копирование началось" << endl;
	int count = 0;
	while (it_source != it_end)
	{
		path ps = getPath(it_source);
		path newpath = copyPath(ps, destinationFolder);

		if (is_directory(*it_source)) {										// папки нас не интересуют
			++it_source;
			continue;
	}
				
		if (!exists(newpath))
		{
			create_directories(newpath.parent_path());
				copy(ps, newpath); count++;
			log << newpath << "\t" << "Был скопирован" << endl;
		}
		else {
			if (last_write_time(ps) > last_write_time(newpath)) {
				copy(ps, newpath, copy_options::overwrite_existing); count++;
				log << newpath << "\t" << "Был заменен" << endl;
			}
		}
		
		++it_source;
}
	
	std::cout << count<< endl;
	if (count == 0) {
		std::cout << "Файлы в обновлении не нуждаются, либо Вы используете старую папку-источник" << endl;
		log << "Файлы в обновлении не нуждаются, либо Вы используете старую папку-источник" << endl;
	}
	else {
		std::cout << "Копирование завершено. log.txt - данные о скопированых файлах." << endl;
		std::cout << "Скопировано " << count << " файлов(а)" << endl;
		log << "Копирование завершено. log.txt - данные о скопированых файлах." << endl;
		log << "Скопировано " << count << " файлов(а)" << endl;
	}
	wcout << source << endl;
	wcout << dest << endl;
	log.close();
	std::system("pause");
    return 0;
} 