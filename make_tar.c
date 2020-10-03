#include <stdio.h>
#include <dirent.h>

struct header
{
	char name[100];
	unsigned int size;
};

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
	dirent* file=readdir(directory);
	printf("%s",(*dirent).d_name); 
	
	return 0;
}
