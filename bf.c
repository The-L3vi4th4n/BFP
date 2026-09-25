#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef DEFAULT_STRIP
#define DEFAULT_STRIP 10000
#endif

#ifndef DEFAULT_PROGRAM
#define DEFAULT_PROGRAM NULL
#endif

int BASE_STRIP = DEFAULT_STRIP;

int main(int args, char *argv[]){
	int size = BASE_STRIP;

	char *program_file = NULL;

	if (args >= 2) {
		program_file = argv[1];
	} else if (DEFAULT_PROGRAM) {
		program_file = DEFAULT_PROGRAM;
	} else {
		printf("bf file not given\n");
		exit(1);
	}

	if (args >= 3) {
		BASE_STRIP = atoi(argv[2]);
	}

	unsigned char *strip = malloc(sizeof(char) * BASE_STRIP);
	if (!strip) {
		printf("couldn't initialise the strip\n");
		exit(1);
	}
	memset(strip, 0, BASE_STRIP * sizeof(unsigned char));
	size = BASE_STRIP;

	FILE *f = fopen(program_file, "r");
	if (!f) {
		printf("file can't be opened: %s\n", program_file);
		exit(1);
	}

	long length = 0;
	fseek(f, 0, SEEK_END);
	length = ftell(f);
	rewind(f);
	char *data = malloc(length + 1);
	if (!data) {
		printf("data can't be loaded\n");
		exit(1);
	}
	size_t bytesRead = fread(data, 1, length, f);
	if (bytesRead != (size_t)length) {
		printf("the file couldn't load\n");
		exit(1);
	}
	data[length] = '\0';

	int ptr = 0;
	unsigned int i = 0;
	char input = '\0';

	while(i < length){
		int loc = 1;
		if (data[i]=='<'){
			ptr--;
			if (ptr<0){
				ptr=size-1;
			}
		} else if (data[i]=='>'){
			ptr++;
			if (ptr>=size){
				ptr=0;
			}
		} else if (data[i]=='+'){
			strip[ptr]++;
		} else if (data[i]=='-'){
			strip[ptr]--;
		} else if (data[i]=='.'){
			printf("%c",(unsigned char)strip[ptr]);
			fflush(stdout);
		} else if (data[i]=='[' && strip[ptr]==0){
			while (1){
				i++;
				if (data[i]=='['){
					loc+=1;
				} else if (data[i]==']'){
					loc-=1;
					if (loc==0) break;
				}
			}
		} else if (data[i]==']' && strip[ptr]!=0){
			while (1){
				i--;
				if (data[i]==']'){
					loc+=1;
				} else if (data[i]=='['){
					loc-=1;
					if (loc==0) break;
				}
			}
		} else if (data[i]==','){
			fflush(stdout);
			input = getchar();
			if (input!=EOF){
				strip[ptr]=(unsigned char)input;
			} else {
				strip[ptr]=0;
			}
		}

		i++;
	}

	fclose(f);
	free(strip);
	free(data);
	return 0;
}
