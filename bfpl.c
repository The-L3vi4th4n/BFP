#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef DEFAULT_STRIP
#define DEFAULT_STRIP 10000
#endif

#ifndef DEFAULT_PTR_MAX
#define DEFAULT_PTR_MAX 100
#endif

#ifndef DEFAULT_PROGRAM
#define DEFAULT_PROGRAM NULL
#endif

int BASE_STRIP = DEFAULT_STRIP;
int PTR_MAX = DEFAULT_PTR_MAX;

int main(int args, char *argv[]){
    int size = BASE_STRIP;
    
    char *program_file = NULL;
    
    if (args >= 2) {
        program_file = argv[1];
    } else if (DEFAULT_PROGRAM) {
        program_file = DEFAULT_PROGRAM;
    } else {
        printf("bfp file not given\n");
        exit(1);
    }
    
    if (args >= 3) {
        BASE_STRIP = atoi(argv[2]);
    }
    
    int *strip = malloc(sizeof(int) * BASE_STRIP);
    if (!strip) {
        printf("couldn't initialise the strip\n");
        exit(1);
    }
    memset(strip, 0, BASE_STRIP * sizeof(int));
    size = BASE_STRIP;
    
    if (args >= 4) {
        PTR_MAX = atoi(argv[3]);
    }
    
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
    unsigned int *tpptr = malloc(sizeof(unsigned int) * PTR_MAX);
    memset(tpptr, 0, PTR_MAX * sizeof(unsigned int));
    unsigned int *lp = malloc(sizeof(unsigned int) * PTR_MAX);
    memset(lp, 0, PTR_MAX * sizeof(unsigned int));
    int dottp = -1;
    unsigned multiply = 1;
    unsigned char input = '\0';
    int inputValue = 0; 
    
    while(i < length){
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
        } else if (data[i]=='^'){
            if (multiply < PTR_MAX && multiply > 0){
                tpptr[multiply-1] = i;
            }
        } else if (data[i]=='*'){
            if (dottp>=0){
                if (dottp < size){
                    lp[dottp]++;
                    if (lp[dottp] < multiply){
                        i = tpptr[dottp];
                        continue;
                    } else {
                        lp[dottp] = 0;
                    }
                }
                dottp=-1;
            } else if (multiply < PTR_MAX && multiply > 0){
                i = tpptr[multiply-1];
                continue;
            }
        } else if (data[i]=='?'){
            if (multiply < PTR_MAX && multiply > 0){
                dottp = multiply-1;
            }
        } else if (data[i]=='$'){
            fflush(stdout);
            if (scanf("%c",&input)==1){
            	if (input!='\n' && input!='\0'){
            		if (input >= '0' && input <= '9'){
            			strip[ptr] = input - '0';
            		}
            		else {
            	    	strip[ptr]=(unsigned char)input;
            		}
            	}
            } else {
                strip[ptr]=0;
            }
            while ((inputValue = getchar()) != '\n' && inputValue != EOF);
        }
        if (data[i] >= '0' && data[i] <= '9'){
            multiply += data[i] - '0';
        } else {
            multiply = 1;
        }
        
        i++;
    }
    
    fclose(f);
    free(lp);
    free(strip);
    free(data);
    free(tpptr);
    return 0;
}
