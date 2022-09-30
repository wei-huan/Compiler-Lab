#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char**argv){
	++argv; --argc;
	if(argc < 1){
		printf("请输入要分析的文件\n");
		exit(1);
	}
	
	FILE* ana_file = fopen(argv[0], "r");

	if(ana_file == NULL){
		printf("要分析的文件不存在\n");
		exit(1);
	}

	int idx = ftell(ana_file);
	printf("idx: %d\n", idx);

	char c;
	
	for(int i = 0; i < 39; i++){
		c = fgetc(ana_file);
		printf("c: %d %c\n", c, c);
	}
	//fread(&c, 1, 1, ana_file);

	idx = ftell(ana_file);
	printf("idx: %d\n", idx);

	fseek(ana_file, -1, SEEK_CUR);
	idx = ftell(ana_file);
	printf("idx: %d\n", idx);

	c = fgetc(ana_file);
	printf("c: %c\n", c);
}
