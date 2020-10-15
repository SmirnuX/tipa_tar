int make_archive(char* fname, char* dir, int depth);	//Архивирует файлы из папки dir в архив fname уровня depth
int add_to_archive(char* fname, FILE* o_file, int depth, char* true_name);	//Добавляет файл fname в архив arc

int unpack_archive(char* fname, char* dir, int depth);	//Распаковывает архив
int unpack_file(FILE* i_file, unsigned long i_size, char *true_name);

