#define BLOCK_SIZE 512	//Размер блока
#define MAX_NAME_LENGTH 100	//Максимальный размер имени файла
#define PERMISSION 00666	//Разрешения

int make_archive(char* fname, char* dir, int depth);	//Архивирует файлы из папки dir в архив fname уровня depth
int add_to_archive(char* fname, int o_file, int depth, char* true_name);	//Добавляет файл fname в архив arc. В заголовочном блоке указывается true_name, если это не пустая строка (используется для именования вложенных папок)

int unpack_archive(char* fname, char* dir, int depth);	//Распаковывает архив
int unpack_file(int i_file, unsigned long i_size, char *true_name);	//Распаковывает один файл из архива. 

