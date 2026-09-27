#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef DEFAULT_STRIP
#define DEFAULT_STRIP 30000
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

	unsigned int i = 0;
	int *table = malloc((length + 1)*4);
	int *temp = malloc((length + 1)*4);
	if (!table || !temp){
		printf("the table couldn't me loaded\n");
		exit(1);
	}
	memset(table, 0, (length + 1)*4);
	while (i<length){
		switch (data[i]){
			case '+':
			case '-':
				{
					int acc = 0;
					while (i<length){ 
						if (data[i]=='+') acc++;
						else if (data[i]=='-') acc--;
						else if (data[i]!='+' && data[i]!='-' && data[i]!='\n' && data[i]!=' ' && data[i]!='\t') break;
						data[i]=' ';
						i++;
					} 
					if (acc>=0) data[i-1]='+';
					else data[i-1]='-';
					table[i-1]=abs(acc);
				}
				break;
			case '<':
			case '>':
				{
					int acc = 0;
					while (i<length){ 
						if (data[i]=='<') acc++;
						else if (data[i]=='>') acc--;
						else if (data[i]!='<' && data[i]!='>' && data[i]!='\n' && data[i]!=' ' && data[i]!='\t') break;
						data[i]=' ';
						i++;
					} 
					if (acc>=0) data[i-1]='<';
					else data[i-1]='>';
					table[i-1]=abs(acc);
				}
				break;
			default:
				i++;
				break;
		}
	}

	int ln = 0;
	memset(temp, 0, (length + 1)*4);

	i = 0;

	while (i < length){
		int tpe = (data[i]=='+' || data[i]=='-' || data[i]=='>' || data[i]=='<' || data[i]=='[' || data[i]==']' || data[i]=='.' || data[i]==',') ? 1 : 0;
		if (tpe) {
			data[ln] = data[i];
			table[ln] = table[i];
			ln++;
		} 
		i++;
	}

	data[ln]='\0';
	length = ln;

	memset(temp, 0, (length + 1)*4);
	int tempPos = 0;
	i = 0;

	while (i < length){
		switch (data[i]){
			case '[':
				{
					temp[tempPos]=i;
					tempPos++;
					i++;
				}
				break;
			case ']':
				{
					tempPos--;
					int curPos = temp[tempPos];
					table[i]=curPos;
					table[curPos]=i;
					i++;
				}
				break;
			default:
				{
					i++;
				}
				break;
		}
	}

	int ptr = 0;
	i = 0;
	char input = '\0';

	while(i < length){
		if (i+2 < length && data[i]=='[' && data[i+2]==']'){
			if (data[i+1]=='+' || data[i+1]=='-'){
				strip[ptr]=0;
				i+=3;
				continue;
			} else if (data[i+1]=='<'){
				while (strip[ptr]!=0){
					ptr=(ptr - (table[i+1] % size) + size) % size;
				}
				i+=3;
				continue;
			} else if (data[i+1]=='>'){
				while (strip[ptr]!=0){
					ptr=(ptr + (table[i+1] % size)) % size;
				}
				i+=3;
				continue;
			}
		}

		switch (data[i]){
			case ' ':
				if (table[i]!=0) i=table[i];
				else i++;
				continue;
			case '<':
				{
					ptr = (ptr - (table[i] % size) + size) % size;
					i++;
					continue;
				} 
			case '>':{
					 ptr = (ptr + (table[i] % size)) % size;
					 i++;
					 continue;
				 }
			case '+':{
					 strip[ptr]+=table[i];
					 i++;
					 continue;
				 } 
			case '-':
				 {
					 strip[ptr]-=table[i];
					 i++;
					 continue;
				 }
			case '.':
				 {
					 printf("%c",(unsigned char)strip[ptr]);
					 fflush(stdout);
					 i++;
					 continue;
				 } 
			case '[': 
				 {
					 if (strip[ptr]==0){
						 i=table[i];
					 } else {
						 i++;
					 }
					 continue;
				 }
			case ']': 
				 { 
					 if (strip[ptr]!=0){
						 i=table[i];
					 } else {
						 i++;
					 }
					 continue;
				 }
			case ',':
				 {
					 input = getchar();
					 if (input!=EOF){
						 strip[ptr]=(unsigned char)input;
					 } else {
						 strip[ptr]=0;
					 }
					 i++;
					 continue;
				 }
			default:
				 {
					 i++;
					 continue;
				 }
		}

	}

	fclose(f);
	free(temp);
	free(strip);
	free(data);
	free(table);
	return 0;
}
