#include "filecopyfunc.h"
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
void printPath(path p1) {
	std::cout << p1 << '\n';
}
wstring check_string(wstring str) {
	if (str[str.length() - 1] == '\\') {
		str.erase(str.end() - 1, str.end());
	}
	return str;
}

path check_str_path(path pth) {
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
			std::cout << "Введеный текст не явялется путём. Попробуйте снова." << endl;
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
	std::cout << "Введите путь исходной папки." << endl;
	while (!exists(sourceFolder)) {
		sourceFolder = get_path();
		if (exists(sourceFolder)) {
			continue;
		}
		std::cout << "Данный путь не существует. Введите новый." << endl;
	}
	std::cout << "Введите путь конечной папки." << endl;
	while (!exists(destinationFolder.parent_path()) || (sourceFolder.stem() != destinationFolder.stem()) || (destinationFolder == sourceFolder)) {
		destinationFolder = get_path();
		bool f = sourceFolder.stem() != destinationFolder.stem();
		if (sourceFolder.stem() != destinationFolder.stem()) {
			std::cout << "Названия папок не идентичны" << endl;
			continue;
		}
		if (sourceFolder == destinationFolder) {
			std::cout << "Эта та же самая папка, чувак" << endl;
			continue;
		}
		if (exists(destinationFolder.parent_path())) {
			continue;
		}
		std::cout << "Промежуточные папки не существуют. Введите новый путь." << endl;
	}





	ofstream log("log.txt", ios_base::trunc);
	log << aTm->tm_hour << ":" << aTm->tm_min << ":" << aTm->tm_sec << " " << aTm->tm_mday << "/" << aTm->tm_mon + 1 << "/" << 1900 + aTm->tm_year << endl;
	recursive_directory_iterator it_source(sourceFolder);
	const recursive_directory_iterator it_end;


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

		if (is_directory(*it_source)) {
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
	log.close();
}