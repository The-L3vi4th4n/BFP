#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef DEFAULT_STRIP
#define DEFAULT_STRIP 30000
#endif

#ifndef DEFAULT_PROGRAM
#define DEFAULT_PROGRAM NULL
#endif

#ifndef FLSH
#define FLSH 64
#endif

unsigned char buffer[FLSH+1];

int BASE_STRIP = DEFAULT_STRIP;

int main(int args, char *argv[]){
	memset(buffer, 0, FLSH+1);

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
	int *table = malloc((length + 1)*(sizeof(int)));
	int *temp = malloc((BASE_STRIP + 1)*(sizeof(int)));
	if (!table || !temp){
		printf("the table couldn't me loaded\n");
		exit(1);
	}
	memset(table, 0, (length + 1)*sizeof(int));
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
					if (acc>0) data[i-1]='+';
					else if (acc<0) data[i-1]='-';
					else data[i-1]=' ';
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
					if (acc>0) data[i-1]='<';
					else if (acc<0) data[i-1]='>';
					else data[i-1]=' ';
					table[i-1]=abs(acc%size);
				}
				break;
			default:
				i++;
				break;
		}
	}

	int ln = 0;
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

	i = 0;

	while (i < length){
		if (i+2 < length && data[i]=='[' && data[i+2]==']'){
			if (data[i+1]=='+' || data[i+1]=='-'){ 
				data[i]=' ';
				table[i]=0;
				data[i+1]='e';
				table[i+1]=0;
				data[i+2]=' ';
				table[i+2]=0;
				i+=3;
				continue;
			} else if (data[i+1]=='<' || data[i+1]=='>'){ 
				data[i]=' ';
				table[i]=0;
				if (data[i+1]=='<') data[i+1]='m';
				else data[i+1]='M';
				data[i+2]=' ';
				table[i+2]=0;
				i+=3;
				continue;
			}
		}
		i++;
	}

	ln = 0;
	i = 0;

	while (i < length){
		int tpe = (data[i]!='\n' && data[i]!='\t' && data[i]!=' ') ? 1 : 0;
		if (tpe) {
			data[ln] = data[i];
			table[ln] = table[i];
			ln++;
		} 
		i++;
	}

	data[ln]='\0';
	length = ln;

	i=0;
	memset(temp, 0, (BASE_STRIP + 1)*(sizeof(int)));

	while (i < length){
		if (data[i]=='['){
			int nxt = 1;
			int j = i+1;
			int offset = 0;
			int ns = 0;
			while (j<length){
				if (data[j]=='[') {
					nxt++;
					ns=1;
				}
				else if (data[j]==']') nxt--;
				else if (data[j]=='<') offset-=table[j];
				else if (data[j]=='>') offset+=table[j];
				if (nxt==0) break;
				j++;
			}
			if (ns==0){
				if (offset==0){
					int hlf = (BASE_STRIP+1)/2;
					int loc = 0;
					int lqc = 0;
					int mx = hlf;
					int mn = hlf;
					int ext = 0;
					for (int k = i; k<j; k++){
						if (data[k]=='>'){
							loc+=table[k];
						} else if (data[k]=='<') loc-=table[k];

						lqc = (loc + hlf)%length;

						if (data[k]=='+') temp[lqc]+=table[k];
						else if (data[k]=='-') temp[lqc]-=table[k];
						else if (data[k]=='.' || data[k]==',') ext=1;
						if ((data[k]=='+' || data[k]=='-') && lqc>mx) mx=lqc;
						if ((data[k]=='+' || data[k]=='-') && lqc<mn) mn=lqc;
					}

					if (ext==0 && temp[hlf]!=0){
						data[i]='c';
						table[i]=temp[hlf];
						i++;
						loc=hlf;
						for (int k=mn; k<=mx; k++){
							if (k==hlf) continue;
							if (k>loc){
								data[i]='>';
								table[i]=abs(k-loc);
								i++;
								loc=k;
							} else if (k<loc){
								data[i]='<';
								table[i]=abs(loc-k);
								i++;
								loc=k;
							}
							if (temp[k]>0){
								data[i]='p';
								table[i]=temp[k];
								i++;
							} else if (temp[k]<0){
								data[i]='s';
								table[i]=abs(temp[k]);
								i++;
							}
						}
						if (hlf<loc){
							data[i]='<';
							table[i]=abs(hlf-loc);
							if (j+1<length){
								if (data[j+1]=='>'){
									table[i]-=table[j+1];
									data[j+1]=' ';
								}
								else if (data[j+1]=='<'){
									table[i]+=table[j+1];
									data[j+1]=' ';
								}
								if (table[i]==0) data[i]=' ';
								else if (table[i]<0){
									table[i]=abs(table[i]);
									data[i]='>';
								}
							}
							i++;
						} else if (hlf>loc){
							data[i]='>';
							table[i]=abs(loc-hlf); 
							if (j+1<length){
								if (data[j+1]=='>'){
									table[i]+=table[j+1];
									data[j+1]=' ';
								}
								else if (data[j+1]=='<'){
									table[i]-=table[j+1];
									data[j+1]=' ';
								}
								if (table[i]==0) data[i]=' ';
								else if (table[i]<0){
									table[i]=abs(table[i]);
									data[i]='<';
								}
							}
							i++;
						}
						while (i<=j){
							data[i]=' ';
							i++;
						}
						for (int k=mn; k<=mx; k++){
							temp[k]=0;
						}
						continue;
					}
				}
			}
			table[i]=j;
			table[j]=i;
		}
		i++;
	}


	ln = 0;
	i = 0;

	while (i < length){
		int tpe = (data[i]!='\n' && data[i]!='\t' && data[i]!=' ') ? 1 : 0;
		if (tpe) {
			data[ln] = data[i];
			table[ln] = table[i];
			ln++;
		} 
		i++;
	}

	data[ln]='\0';
	length = ln;

	data[ln]='\0';
	length = ln;

	int ptr = 0;
	i = 0;
	char input = '\0';
	int multiply = 0;
	int flsh = 0;

	while(i < length){
		switch (data[i]){
			case 'c':
				{
					multiply=0;
					while (strip[ptr]!=0){
						strip[ptr]+=table[i];
						multiply++;
					}
					i++;
					continue;
				}
			case 'p':
				{
					strip[ptr]+=table[i]*multiply;
					i++;
					continue;
				}
			case 's':
				{
					strip[ptr]-=table[i]*multiply;
					i++;
					continue;
				}
			case 'e':
				{
					strip[ptr]=0;
					i++;
					continue;
				}
			case 'm':
				{
					int mve = (table[i] % size);
					while (strip[ptr]!=0){
						//ptr=(ptr - mve + size) % size;
						ptr-=mve;
						if (ptr<0) ptr+=size;
					}
					i++;
					continue;

				}

			case 'M':
				{
					int mve = (table[i] % size);
					while (strip[ptr]!=0){
						//ptr=(ptr + mve) % size;
						ptr+=mve;
						if (ptr>=size) ptr=ptr-size;
					}
					i++;
					continue;

				}
			case '<':
				{
					ptr = ptr - table[i];
					if (ptr<0) ptr+=size;
					i++;
					continue;
				} 
			case '>':{
					 ptr = ptr + table[i];
					 if (ptr>=size) ptr=ptr-size;
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
					 buffer[flsh]=strip[ptr];
					 flsh++;
					 if (flsh==FLSH || strip[ptr]=='\n'){
						 fwrite(buffer, 1, flsh, stdout);
						 fflush(stdout);
						 flsh=0;
					 }
					 i++;
					 continue;
				 } 
			case '[': 
				 {
					 if (strip[ptr]==0){
						 i=table[i]+1;
					 } else {
						 i++;
					 }
					 continue;
				 }
			case ']': 
				 { 
					 if (strip[ptr]!=0){
						 i=table[i]+1;
					 } else {
						 i++;
					 }
					 continue;
				 }
			case ',':
				 {
					 printf("%s", buffer);
					 flsh=0;
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
