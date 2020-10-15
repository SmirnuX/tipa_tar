#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "tipatar.h"

int make_archive(char* fname, char* dir, int depth)	//Архивирует файлы из папки dir в архив fname уровня depth
{
	FILE* o_file=fopen(fname, "wb");
	if (o_file==NULL)
	{
		printf("Не получается создать архив: %s", fname);
		return 1;
	}
	DIR* directory = opendir(dir);
	if (directory==NULL)
	{
		printf("Не получается открыть директорию: %s", dir);
		return 2;
	}
	chdir(dir);	//Переходим в эту директорию
	struct dirent* dir_ptr;
	struct stat stat_file;
	dir_ptr=readdir(directory);
	while (dir_ptr!=NULL)
	{
		if (strcmp((*dir_ptr).d_name, ".")!=0 && strcmp((*dir_ptr).d_name, "..")!=0)	//Пропускаем точки и двойные точки
		{
			printf("> %s",(*dir_ptr).d_name);
			//Проверяем на то, является ли файл папкой
			lstat((*dir_ptr).d_name, &stat_file);
			if (S_ISDIR(stat_file.st_mode))
			{
				printf(" - папка.\n");
				make_archive(".tempttar", (*dir_ptr).d_name, depth+1);	//Архивируем вложенную папку
				add_to_archive(".tempttar", o_file, depth+1, (*dir_ptr).d_name);	//Записываем ее в архив
				remove(".tempttar");	//Удаляем временный файл
			}
			else
			{
				printf(" - файл.\n");
				add_to_archive((*dir_ptr).d_name, o_file, depth, "");	//Записываем файл в архив
			}
		}
		dir_ptr=readdir(directory);	//Переходим к следующему файлу
	}
	chdir("..");	//Возвращаемся на уровень выше
	closedir(directory);
	fclose(o_file);
}


int add_to_archive(char* fname, FILE* o_file, int depth, char* true_name)	//Добавляет файл fname в архив arc. В заголовочном блоке архива указывается либо true_name, либо fname (если true_name=="")
{
	//Блок заголовка
	if (strlen(fname)>99)
	{
		printf("Слишком длинное название файла\n");
		return 1;
	}
	unsigned long size;
	struct stat stats;
	FILE* i_file=fopen(fname, "rb");
	if (i_file==NULL)
		printf("Не удалось открыть файл %s", fname);
	lstat(fname, &stats);
	size=stats.st_size;
	printf("Размер: %lu\n",size); 
	if (strcmp("", true_name)==0)
		fwrite(fname, 1, 100, o_file);	//Запись имени
	else
		fwrite(true_name, 1, 100, o_file);	//Запись имени папки
	fwrite(&size, 1, sizeof(long), o_file);	//Запись размера файла
	fwrite(&depth, 1, sizeof(int), o_file);	//Запись глубины залегания файла
	char a=0;
	for (int i=0; i<412-sizeof(int)-sizeof(long); i++)
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
	return 0;
}
