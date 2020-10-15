#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "tipatar.h"

int unpack_archive(char* fname, char* dir, int depth)	//Распаковывает архив
{
	//Откроем архив
	FILE* i_file=fopen(fname, "rb");
	if (i_file==NULL)
	{
		printf("Файл не создан");
		return 1;
	}
	//Проверим наличие папки
	DIR* directory = opendir(dir);
	if (directory==NULL)
	{
		//Создадим папку
		if (mkdir(dir, S_IRUSR|S_IWUSR|S_IXUSR)==-1)	//TODO - рассмотреть подробнее права доступа
		{
			printf("Не удается открыть/создать папку %s", dir);
			return 2;
		}
	}
	closedir(directory);
	chdir(dir);
	//Начинаем распаковывать файлы
	unsigned long i_size;
	int i_depth;
	char i_name[100];
	char a;
	while(fread(i_name, 1, 100, i_file)==100)
	{
		fread(&i_size, 1, sizeof(long), i_file);	//Считывание размера файла
		fread(&i_depth, 1, sizeof(int), i_file);	//Считывание глубины залегания файла
		for (int i=0; i<412-sizeof(int)-sizeof(long); i++)
		{
			fread(&a, 1, 1, i_file);	//Считывание заголовочного блока полностью
		}
		//Проверяем, является ли считанный файл собственно файлом или архивом (т.е. вложенной папкой)
		if (i_depth==depth+1)
		{
			//Распаковка вложенного архива
			unpack_file(i_file, i_size, ".included_archive");
			unpack_archive(".included_archive", i_name, depth+1);
			remove(".included_archive");
		}
		else if (i_depth==depth)
		{
			unpack_file(i_file, i_size, i_name);
		}
		else
		{
			printf("Ошибка архива - несовпадение глубин\n");
			return 3;
		}
	}
	chdir("..");	//Возвращаемся на уровень выше
	return 0;
}


int unpack_file(FILE* i_file, unsigned long i_size, char *true_name)
{
	char i_block[512];
	FILE* n_file=fopen(true_name, "wb");	//Создание файла
	printf("Распаковка %s\n", true_name);
	//Заполнение файла
	for(int i=0; i<i_size; )
	{
		//Считываем блок
		int count=fread(i_block, 1, 512, i_file);
		printf("%i/%lu - \n",i,i_size);
		//Записываем блок
		if ((i+512)>i_size)
		{
			fwrite(i_block, 1, i_size-i, n_file);
			break;
		}
		else
		{
			fwrite(i_block, 1, 512, n_file);
			i+=512;
		}
	}
	fclose(n_file);
}

/*
int main(int argc, char* argv[])
{
	if (argc<2)
	{
		printf("There have to be one argument");
		return 1;
	}
	FILE* arc_file=fopen(argv[1],"rb");
	FILE* n_file;
	char block[512];
	char name[100];
	char a;
	int count;
	printf("<\n");
	//while(1)
	{
		//Считывание заголовка
		fread(name, 1, 100, arc_file);
		printf("%s\n", name);
		unsigned int size;
		fread(&size, 1, sizeof(int), arc_file);
		printf("%u\n", size);
		for (int i=0; i<412-sizeof(int); i++)
			fread(&a, 1, 1, arc_file);
		
		//Создание файла
		n_file=fopen(name, "wb");
		//Заполнение файла
		for(int i=0; i<size; )
		{
			//Считываем блок
			count=fread(block, 1, 512, arc_file);
			printf("%i/%u - ",i,size);
			//Записываем блок
			if ((i+512)>size)
			{
				printf("%i\n",size-i);
				fwrite(block, 1, size-i, n_file);
				break;
			}
			else
			{
				printf("512\n");
				fwrite(block, 1, 512, n_file);
				i+=512;
			}
		}
	}
	printf("\n>");

	return 0;
}
*/
