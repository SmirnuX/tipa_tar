#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include "tipatar.h"



const int mode=1;	//Включен ли режим отладки

int main(int argc, char* argv[])
{
	if (argc<4)
	{
		printf("Необходимо три аргумента:\n\t - Режим архивирования: \
		-a <папка, содержащая файлы> <название архива>\n\t - \
		Режим разархивирования: \
		-r <название архива> <папка, в которую будет разархивироваться>\n");
		return 1;
	}
	if (strcmp(argv[1], "-a")==0)
	{
		//Запаковка
		make_archive(argv[3], argv[2], 0);
	}
	else if (strcmp(argv[1], "-r")==0)
	{
		//Распаковка
		unpack_archive(argv[2], argv[3], 0);
	}
	else
	{
		printf("Неожиданный аргумент. Первый аргумент должен быть либо -a, либо -r");
	}
	return 0;
}
