#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

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
