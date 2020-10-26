#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "tipatar.h"

#include <errno.h>


int make_archive(char* fname, char* dir, int depth)	//Архивирует файлы из папки dir в архив fname уровня depth
{
	int o_file=open(fname, O_CREAT|O_WRONLY, 00666);
	if (o_file==-1)
	{
		printf("Не получается создать архив: %s", fname);
		return 1;
	}
	perror("");
	DIR* directory = opendir(dir);
	/*if (directory==NULL)
	{
		printf("Не получается открыть директорию: %s", dir);
		return 2;
	}*/
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
	if (close(o_file)==-1)
	{
		//TODO - ERROR
	}
}


int add_to_archive(char* fname, int o_file, int depth, char* true_name)	//Добавляет файл fname в архив arc. В заголовочном блоке архива указывается либо true_name, либо fname (если true_name=="")
{
	//Блок заголовка
	if (strlen(fname)>MAX_NAME_LENGTH)
	{
		printf("Слишком длинное название файла\n");
		return 1;
	}
	unsigned long size;
	struct stat stats;
	int i_file=open(fname, O_RDONLY);
	if (i_file==-1)
	{
		printf("Не удалось открыть файл %s", fname);
	}
	lstat(fname, &stats);
	size=stats.st_size;
	printf("Размер: %lu\n",size); 
	if (strcmp("", true_name)==0)
	{
		if (write(o_file, fname, MAX_NAME_LENGTH)!=MAX_NAME_LENGTH);	//Запись имени
			perror("");
	}
	else
		write(o_file, true_name, MAX_NAME_LENGTH);	//Запись имени папки
	write(o_file, &size, sizeof(long));	//Запись размера файла
	write(o_file, &depth, sizeof(int));	//Запись глубины залегания файла
	char a=0;
	for (int i=0; i<BLOCK_SIZE-MAX_NAME_LENGTH-sizeof(int)-sizeof(long); i++)
		write(o_file, &a, 1);	//Заполняем блок из BLOCK_SIZE байт до конца
	//Блоки данных
	unsigned char block[BLOCK_SIZE];
	int count=read(i_file, block, BLOCK_SIZE);
	while(count==BLOCK_SIZE)
	{
		write(o_file, block, BLOCK_SIZE);
		count=read(i_file, block, BLOCK_SIZE);
	}
	if (count!=0)
	{
		write(o_file, block, count);
		for (int i=0; i<BLOCK_SIZE-count; i++)	//Дописываем блок до BLOCK_SIZE байт
			write(o_file, &a, 1);
	}
	close(i_file);
	return 0;
}
