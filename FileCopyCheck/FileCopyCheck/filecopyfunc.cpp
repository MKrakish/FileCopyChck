
#include "stdafx.h"
#include "filecopyfunc.h"
using namespace std;
using namespace std::tr2::sys;

wstring get_value(wstring source, wstring item) {
	int start_it = source.find(item) + item.length() + 1;
	int end_it = source.find_first_of(';', start_it);
	if (end_it == -1) {
		end_it = source.find_last_of('"');
	}
	return source.substr(start_it, end_it - start_it);
}

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
	int start = filepath.wstring().rfind(folderName, filepath.wstring().length())+folderName.length()-0;
	wstring addpath = filepath.wstring().substr(start);
	path cpPath(destpath.wstring() + addpath);
	return cpPath;
}

void copyFolder(wstring dest) {
	
	wstring source; path sourceFolder;
	//wstring dest;
	path destinationFolder;
	int rcount = 0;
	ofstream log("log.txt", ios_base::app);
	std::cout << "Введите путь исходной папки." << endl;
	while (!exists(sourceFolder)) {
		sourceFolder = get_path();
		if (exists(sourceFolder)) {
			continue;
		}
		std::cout << "Данный путь не существует. Введите новый." << endl;
	}
	//std::cout << "Введите путь конечной папки." << endl;
	//while (!exists(destinationFolder.parent_path()) || (sourceFolder.stem() != destinationFolder.stem()) || (destinationFolder == sourceFolder)) {
		destinationFolder = path(dest);
		bool f = sourceFolder.stem() != destinationFolder.stem();
		if (sourceFolder.stem() != destinationFolder.stem()) {
			std::cout << "Названия папок не идентичны" << endl;
			std::cout << sourceFolder.stem() << " " << destinationFolder.stem() << endl;
			return;
			//continue; 
		}
		if (sourceFolder == destinationFolder) {
			std::cout << "Эта та же самая папка, чувак" << endl;
			return;
			//continue;
		}
		if (exists(destinationFolder.parent_path())) {
			//continue;
		}
		//std::cout << "Промежуточные папки не существуют. Введите новый путь." << endl;
	//}





	
	recursive_directory_iterator it_source(sourceFolder);
	const recursive_directory_iterator it_end;


	if (!exists(destinationFolder)) {
		//std::cout << destinationFolder << endl;
		create_directory(destinationFolder);
	}


	
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
			std::cout << newpath << endl;
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