#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

int archive_file(char* fname, FILE* o_file, char* dir)
{
	//Блок заголовка
	if (strlen(fname)>99)
	{
		printf("Слишком длинное название файла\n");
		return 1;
	}
	char name[100];
	char path[200];
	unsigned int size;
	struct stat stats;
	//Собираем путь к файлу
	strcpy(path, dir);
	strcat(path, "/");
	strcat(path, fname);
	strcpy(name, fname);
	FILE* i_file=fopen(path, "rb");
	if (i_file==NULL)
		printf("eee");
	//Вставить проверку на открытие
	stat(path,&stats);
	size=stats.st_size;
	printf("Размер: %u\n",size); 
	fwrite(name, 1, 100, o_file);	//Запись имени
	fwrite(&size, 1, sizeof(int), o_file);	//Запись размера файла
	char a=0;
	for (int i=0; i<412-sizeof(int); i++)
	{
		fwrite(&a, 1, 1, o_file);	//Заполняем блок из 512 байт до конца
	}
	//Блоки данных
	char block[512];
	int count=fread(block, 1, 512, i_file);
	while(count==512)
	{
		fwrite(block, 1, 512, o_file);
		count=fread(block, 1, 512, i_file);
	}
	if (count!=0)
	{
		fwrite(block, 1, count, o_file);
		//Дописываем блок до 512 байт
		for (int i=0; i<512-count; i++)
			fwrite(&a, 1, 1, o_file);
	}
	//Вставляем два пустых блока
	for (int i=0; i<1024; i++)
		fwrite(&a, 1, 1, o_file);
	return 0;
}

int main(int argc, char* argv[])
{
	if (argc<2)
	{
		printf("There have to be one argument");
		return 1;
	}
	DIR* directory = opendir(argv[1]);
	if (directory==NULL)
	{
		printf("Cannot open the directory");
		return 2;
	}
	//Пока пусть будет без вложенных папок
	char arc_name[100];
	char path[100];
	strcpy(path, argv[1]);
	strcpy(arc_name, argv[1]);
	strcat(arc_name, ".tipatar");
	FILE* file=fopen(arc_name, "wb");
	struct dirent* dirf_ptr;
	dirf_ptr=readdir(directory);
	while (dirf_ptr!=NULL)
	{
		if (strcmp((*dirf_ptr).d_name, ".")!=0 && strcmp((*dirf_ptr).d_name, "..")!=0)	//Пропускаем точки
		{
			printf("Архивируем %s...\n",(*dirf_ptr).d_name); 
			archive_file((*dirf_ptr).d_name, file, argv[1]);
		}
		dirf_ptr=readdir(directory);
	}
	return 0;
}
