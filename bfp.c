#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#ifdef _WIN32
#include <conio.h>
char getch() {
    return _getch();
}
#else

#include <unistd.h>
#include <termios.h>

char getch(void) {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}
#endif 

// Source for the function above - https://stackoverflow.com/a/912796
// Posted by anon, modified by community. See post 'Timeline' for change history
// Retrieved 2026-03-27, License - CC BY-SA 3.0

#ifndef __APPLE__
unsigned int arc4random_uniform(unsigned int x){
    static int seedy=0;
    if (seedy==0){
        srand(time(NULL));
        seedy=1;
    }
    return(rand()%x);
}
#endif

#ifndef DEFAULT_STRIP
#define DEFAULT_STRIP 10000
#endif

#ifndef DEFAULT_PTR_MAX
#define DEFAULT_PTR_MAX 100
#endif

#ifndef DEFAULT_PROGRAM
#define DEFAULT_PROGRAM NULL
#endif

#ifndef DEFAULT_OUTPUT
#define DEFAULT_OUTPUT NULL
#endif

int BASE_STRIP = DEFAULT_STRIP;
int PTR_MAX = DEFAULT_PTR_MAX;

int *rbracket(int *strip, int location, int multiply, int length, char data[], int ptr);
int *sbracket(int *strip, int location, int multiply, int length, char data[], int ptr);

int main(int args, char *argv[]){
    int size = BASE_STRIP;
    
    char *program_file = NULL;
    char *output_file = DEFAULT_OUTPUT;
    
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
    
    if (args >= 5) {
        output_file = argv[4];
    }
    
    if (output_file) {
        FILE *l = fopen(output_file, "wb");
        if (!l) {
            printf("output file can't be opened\n");
            exit(1);
        }
        fwrite(&strip[0], sizeof(int), 1, l);
        fclose(l);
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
    if (bytesRead != length) {
        printf("the file couldn't load\n");
        exit(1);
    }
    data[length] = '\0';
    
    int ptr = 0;
    unsigned int i = 0;
    unsigned int pvptr = 0;
    unsigned int *tpptr = malloc(sizeof(unsigned int) * PTR_MAX);
    memset(tpptr, 0, PTR_MAX * sizeof(unsigned int));
    unsigned int *lp = malloc(sizeof(unsigned int) * PTR_MAX);
    memset(lp, 0, PTR_MAX * sizeof(unsigned int));
    unsigned int dottp = 0;
    unsigned multiply = 1;
    unsigned nl_multiply = 0;
    unsigned int io = 0;
    unsigned int pos = 0;
    unsigned int dpos = 0;
    unsigned int depth = 0;
    unsigned char input = '\0';
    
    int *br_o;
    
    unsigned rbr_multiply = 0;
    unsigned int rbr = 0;
    
    unsigned sbr_multiply = 0;
    unsigned int sbr = 0;
    
    unsigned char letter = '\0';
    int range = 0;
    int inputValue = 0;

    int use_ptr_mode=0;
    int index=0;
    
    while(i < length){
        if (data[i]=='<'){
            for (int k=0; k<multiply; k++){
            	ptr--;    	
	            if (ptr<0){
	            	ptr=size-1;
	            }
            }
        } else if (data[i]=='>'){
            for (int k=0; k<multiply; k++){
            	ptr++;
	            if (ptr>=size){
	            	ptr=0;
	            }
            }
        } else if (data[i]=='+'){
            for (int k=0; k<multiply; k++){
            	strip[ptr]++;
            }
        } else if (data[i]=='-'){
            for (int k=0; k<multiply; k++){
            	strip[ptr]--;
            }
        } else if (data[i]=='.'){
            letter = (unsigned char)strip[ptr];
            for (int k=0; k<multiply; k++){
            	printf("%c",(unsigned char)letter);
            }
        } else if (data[i]==','){
            for (int k=0; k<multiply; k++){
            	printf("%d",strip[ptr]);
            }
        } else if (data[i]==':'){
            pvptr=ptr;
            ptr=0;
        } else if (data[i]==';'){
            pvptr=ptr;
            ptr=size-1;
        } else if (data[i]=='~'){
            ptr=pvptr;
        } else if (data[i]=='_'){
            fflush(stdout);
        } else if (data[i]=='%'){
            for (int k=0; k<multiply; k++){
            	puts("");
            }
        } else if (data[i]=='^'){
            if (multiply < PTR_MAX && multiply > 0){
                tpptr[multiply-1] = i;
            }
        } else if (data[i]=='*'){
            if (multiply < PTR_MAX && multiply > 0){
                i = tpptr[multiply-1];
                continue;
            }
        } else if (data[i]=='@'){
            usleep(multiply*1000);
        } else if (data[i]=='!'){
            printf("\033[2J\033[H");
            fflush(stdout);
        } else if (data[i]=='|'){
            if (use_ptr_mode) {
                index = ptr;
            } else {
                index = multiply - 1;
            }
            if (index >= 0 && index < size) {
                nl_multiply = strip[index];
            }
        } else if (data[i]=='`'){
        	break;
        } else if (data[i]=='\\'){
            use_ptr_mode=0;
        } else if (data[i]=='?'){
            if (multiply < PTR_MAX && multiply > 0){
                dottp = multiply-1;
            }
        } else if (data[i]=='#'){
            if (dottp < PTR_MAX){
                lp[dottp]++;
                if (lp[dottp] < multiply){
                    i = tpptr[dottp];
                    continue;
                } else {
                    lp[dottp] = 0;
                }
            }
        } else if (data[i]=='$'){
            for (int j=0; j<multiply && ptr+j < size; j++){
                if (scanf("%c",&input)==1){
                	if (input!='\n' && input!='\0'){
                		if (input >= '0' && input <= '9'){
                			strip[ptr+j] = input - '0';
                		}
                		else {
                	    	strip[ptr+j]=(unsigned char)input;
                		}
                	}
                } else {
                    strip[ptr+j]=0;
                }
            }
            while ((inputValue = getchar()) != '\n' && inputValue != EOF);
        } else if (data[i]=='&'){
            for (int j=0; j<multiply && ptr+j < size; j++){
            	input=getch();
            	if (input!='\n' && input!='\0'){
            		if (input >= '0' && input <= '9'){
            			strip[ptr+j] = input - '0';
            		}
            		else {
            	    	strip[ptr+j]=(unsigned char)input;
            		}
            	} else {
                    strip[ptr+j]=0;
                }
            }
        } else if (data[i]=='='){
            range = multiply;
            if (range <= 0) range = 1;
            strip[ptr] = arc4random_uniform(range);
        } else if (data[i]=='('){
            br_o = rbracket(strip, i, multiply, size, data, ptr);
            rbr = 1;
            strip[ptr] = br_o[0];
            rbr_multiply = br_o[0];
            i = br_o[1];
        } else if (data[i]=='['){
            br_o = sbracket(strip, i, multiply, size, data, ptr);
            sbr = 1;
            sbr_multiply = br_o[0];
            i = br_o[1];
        } else if (data[i]=='{'){
            if (sbr_multiply == 0 && sbr == 1){
                sbr = 0;
                i++;
                continue;
            } else {
                io = i;
                pos = i;
                depth = 0;
                for (int j = 1; pos < length; j++){
                    pos = j + i;
                    if (data[pos] == '}' && depth == 0){
                        dpos = pos;
                        break;
                    } else if (data[pos] == '}' && depth > 0){
                        depth--;
                    } else if (data[pos] == '{'){
                        depth++;
                    }
                }
                if (io == dpos){
                    printf("error, no closing bracket at line %d\n", i);
                    exit(1);
                } else {
                    i = dpos;
                    dpos = 0;
                }
            }
        } else if (data[i] == '\'') {
            if (multiply - 1 == 0){
                memset(strip, 0, size * sizeof(int));
            } else {
                for (int k = 0; k < multiply - 1 && ptr + k < size; k++) {
                    strip[ptr + k] = 0;
                }
            }
        } else if (data[i] == '"') {
            if (multiply - 1 == 0){
                memset(strip, dottp, size * sizeof(int));
            } else {
                for (int k = 0; k < multiply - 1 && ptr + k < size; k++) {
                    strip[ptr + k] = dottp;
                }
            }
        } else if (data[i]=='/'){
            use_ptr_mode=1;
        }
        
        if (data[i] >= '0' && data[i] <= '9'){
            multiply += data[i] - '0';
        } else {
            multiply = 1;
            multiply += nl_multiply;
        }
        
        if (rbr){
            multiply = rbr_multiply;
            rbr = 0;
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

int *sbracket(int *strip, int location, int multiply, int length, char data[], int ptr){
    static int retd[2];
    int ret = 0;
    int dottp = 0;
    int mult = 1;
    int nl_multiply=0;
    int val1 = 0;
    int val2 = 0;
    location++;
    int *nested;
    int mode = 0;
    int separator_hit = 0;
    int use_ptr_mode=0;
    int index = 0;
    
    while (location < length) {
        if (data[location] == ']') {
            break;
        } else if (data[location] == '(') {
            nested = rbracket(strip, location, mult, length, data, ptr);
            ret += nested[0];
            location = nested[1] + 1;
            continue;
        } else if (data[location] == '[') {
            nested = sbracket(strip, location, mult, length, data, ptr);
            ret += nested[0];
            location = nested[1] + 1;
            continue;
        } else if (data[location] == '|') {
            if (use_ptr_mode) {
                index = ptr;
            } else {
                index = mult - 1;
            }
            if (index >= 0 && index < BASE_STRIP) {
                nl_multiply = strip[index];
            }
        } else if (data[location]=='?'){
            if (mult < PTR_MAX && mult > 0){
                dottp = mult-1;
            }
        } else if (data[location] == '+') {
            ret += mult;
        } else if (data[location] == '-') {
            ret -= mult;
        } else if (data[location] == ',') {
            mode = 0;
            val1 = ret;
            ret = 0;
            separator_hit = 1;
            location++;
            continue;
        } else if (data[location] == '.') {
            mode = 1;
            val1 = ret;
            ret = 0;
            separator_hit = 1;
            location++;
            continue;
        } else if (data[location] == ';') {
            mode = 2;
            val1 = ret;
            ret = 0;
            separator_hit = 1;
            location++;
            continue;
        } else if (data[location] == ':') {
            val2 = ret;
            ret = 0;
            location++;
            continue;
        }  else if (data[location]=='/'){
            use_ptr_mode=1;
        } else if (data[location]=='\\'){
            use_ptr_mode=0;
        }
        if (data[location] >= '0' && data[location] <= '9'){
            mult += data[location] - '0';
        } else {
            mult = 1;
            mult += nl_multiply;
        }
        location++;
    }
    
    if (!separator_hit) {
        retd[0] = (ret != 0) ? 0 : 1;
    } else if (mode == 0) {
        retd[0] = (val1 == ret) ? 0 : 1;
    } else if (mode == 1) {
        retd[0] = (val1 != ret) ? 0 : 1;
    } else {
        retd[0] = (val1 == ret || val2 == ret) ? 0 : 1;
    }
    
    retd[1] = location;
    return retd;
}

int *rbracket(int *strip, int location, int multiply, int length, char data[], int ptr){
    static int retd[2];
    int dottp=0;
    int ret = 0;
    int mult = 1;
    int nl_multiply=0;
    location++;
    int *nested;
    int use_ptr_mode=0;
    int index = 0;
    
    while (location < length) {
        if (data[location] == ')') {
            break;
        } else if (data[location] == '(') {
            nested = rbracket(strip, location, mult, length, data, ptr);
            ret += nested[0];
            location = nested[1] + 1;
            continue;
        } else if (data[location] == '|') {
            if (use_ptr_mode) {
                index = ptr;
            } else {
                index = mult - 1;
            }
            if (index >= 0 && index < BASE_STRIP) {
                nl_multiply = strip[index];
            }
        } else if (data[location] == '+') {
            ret += mult;
        } else if (data[location]=='?'){
            if (mult < PTR_MAX && mult > 0){
                dottp = mult-1;
            }
        } else if (data[location] == '-') {
            ret -= mult;
      	} else if (data[location]=='/'){
            use_ptr_mode=1;
        } else if (data[location]=='\\'){
            use_ptr_mode=0;
        }
        if (data[location] >= '0' && data[location] <= '9'){
            mult += data[location] - '0';
        } else {
            mult = 1;
            mult += nl_multiply;
        }
        location++;
    }
    
    retd[0] = ret;
    retd[1] = location;
    return retd;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#ifdef _WIN32
#include <conio.h>
char getch() {
    return _getch();
}
#else

#include <unistd.h>
#include <termios.h>

char getch(void) {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}
#endif 

// Source for the function above - https://stackoverflow.com/a/912796
// Posted by anon, modified by community. See post 'Timeline' for change history
// Retrieved 2026-03-27, License - CC BY-SA 3.0

#ifndef __APPLE__
unsigned int arc4random_uniform(unsigned int x){
    static int seedy=0;
    if (seedy==0){
        srand(time(NULL));
        seedy=1;
    }
    return(rand()%x);
}
#endif

#ifndef DEFAULT_STRIP
#define DEFAULT_STRIP 10000
#endif

#ifndef DEFAULT_PTR_MAX
#define DEFAULT_PTR_MAX 100
#endif

#ifndef DEFAULT_PROGRAM
#define DEFAULT_PROGRAM NULL
#endif

#ifndef DEFAULT_OUTPUT
#define DEFAULT_OUTPUT NULL
#endif

int BASE_STRIP = DEFAULT_STRIP;
int PTR_MAX = DEFAULT_PTR_MAX;

int *rbracket(int *strip, int location, int multiply, int length, char data[]);
int *sbracket(int *strip, int location, int multiply, int length, char data[]);

int main(int args, char *argv[]){
    int size = BASE_STRIP;
    
    char *program_file = NULL;
    char *output_file = DEFAULT_OUTPUT;
    
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
    
    if (args >= 5) {
        output_file = argv[4];
    }
    
    if (output_file) {
        FILE *l = fopen(output_file, "wb");
        if (!l) {
            printf("output file can't be opened\n");
            exit(1);
        }
        fwrite(&strip[0], sizeof(int), 1, l);
        fclose(l);
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
    if (bytesRead != length) {
        printf("the file couldn't load\n");
        exit(1);
    }
    data[length] = '\0';
    
    int ptr = 0;
    unsigned int i = 0;
    unsigned int pvptr = 0;
    unsigned int *tpptr = malloc(sizeof(unsigned int) * PTR_MAX);
    memset(tpptr, 0, PTR_MAX * sizeof(unsigned int));
    unsigned int *lp = malloc(sizeof(unsigned int) * PTR_MAX);
    memset(lp, 0, PTR_MAX * sizeof(unsigned int));
    unsigned int dottp = 0;
    unsigned multiply = 1;
    unsigned nl_multiply = 0;
    unsigned int io = 0;
    unsigned int pos = 0;
    unsigned int dpos = 0;
    unsigned int depth = 0;
    unsigned char input = '\0';
    
    int *br_o;
    
    unsigned rbr_multiply = 0;
    unsigned int rbr = 0;
    
    unsigned sbr_multiply = 0;
    unsigned int sbr = 0;
    
    unsigned char letter = '\0';
    int range = 0;
    int inputValue = 0;
    
    while(i < length){
        if (data[i]=='<'){
            for (int k=0; k<multiply; k++){
            	ptr--;    	
	            if (ptr<0){
	            	ptr=size-1;
	            }
            }
        } else if (data[i]=='>'){
            for (int k=0; k<multiply; k++){
            	ptr++;
	            if (ptr>=size){
	            	ptr=0;
	            }
            }
        } else if (data[i]=='+'){
            for (int k=0; k<multiply; k++){
            	strip[ptr]++;
            }
        } else if (data[i]=='-'){
            for (int k=0; k<multiply; k++){
            	strip[ptr]--;
            }
        } else if (data[i]=='.'){
            letter = (unsigned char)strip[ptr];
            for (int k=0; k<multiply; k++){
            	printf("%c",(unsigned char)letter);
            }
        } else if (data[i]==','){
            for (int k=0; k<multiply; k++){
            	printf("%d",strip[ptr]);
            }
        } else if (data[i]==':'){
            pvptr=ptr;
            ptr=0;
        } else if (data[i]==';'){
            pvptr=ptr;
            ptr=size-1;
        } else if (data[i]=='~'){
            ptr=pvptr;
        } else if (data[i]=='_'){
            fflush(stdout);
        } else if (data[i]=='%'){
            for (int k=0; k<multiply; k++){
            	puts("");
            }
        } else if (data[i]=='^'){
            if (multiply < PTR_MAX && multiply > 0){
                tpptr[multiply-1] = i;
            }
        } else if (data[i]=='*'){
            if (multiply < PTR_MAX && multiply > 0){
                i = tpptr[multiply-1];
                continue;
            }
        } else if (data[i]=='@'){
            usleep(multiply*1000);
        } else if (data[i]=='!'){
            printf("\033[2J\033[H");
            fflush(stdout);
        } else if (data[i]=='|'){
            if (multiply > 0 && multiply <= size) {
                nl_multiply = strip[multiply-1];
            }
        } else if (data[i]=='`'){
        	break;
        } else if (data[i]=='\\'){
            nl_multiply = 0;
        } else if (data[i]=='?'){
            if (multiply < PTR_MAX && multiply > 0){
                dottp = multiply-1;
            }
        } else if (data[i]=='#'){
            if (dottp < PTR_MAX){
                lp[dottp]++;
                if (lp[dottp] < multiply){
                    i = tpptr[dottp];
                    continue;
                } else {
                    lp[dottp] = 0;
                }
            }
        } else if (data[i]=='$'){
            for (int j=0; j<multiply && ptr+j < size; j++){
                if (scanf("%c",&input)==1){
                	if (input!='\n' && input!='\0'){
                		if (input >= '0' && input <= '9'){
                			strip[ptr+j] = input - '0';
                		}
                		else {
                	    	strip[ptr+j]=(unsigned char)input;
                		}
                	}
                } else {
                    strip[ptr+j]=0;
                }
            }
            while ((inputValue = getchar()) != '\n' && inputValue != EOF);
        } else if (data[i]=='&'){
            for (int j=0; j<multiply && ptr+j < size; j++){
            	input=getch();
            	if (input!='\n' && input!='\0'){
            		if (input >= '0' && input <= '9'){
            			strip[ptr+j] = input - '0';
            		}
            		else {
            	    	strip[ptr+j]=(unsigned char)input;
            		}
            	} else {
                    strip[ptr+j]=0;
                }
            }
        } else if (data[i]=='='){
            range = multiply;
            if (range <= 0) range = 1;
            strip[ptr] = arc4random_uniform(range);
        } else if (data[i]=='('){
            br_o = rbracket(strip, i, multiply, size, data);
            rbr = 1;
            strip[ptr] = br_o[0];
            rbr_multiply = br_o[0];
            i = br_o[1];
        } else if (data[i]=='['){
            br_o = sbracket(strip, i, multiply, size, data);
            sbr = 1;
            strip[ptr] = br_o[0];
            sbr_multiply = br_o[0];
            i = br_o[1];
        } else if (data[i]=='{'){
            if (sbr_multiply == 0 && sbr == 1){
                sbr = 0;
                i++;
                continue;
            } else {
                io = i;
                pos = i;
                depth = 0;
                for (int j = 1; pos < length; j++){
                    pos = j + i;
                    if (data[pos] == '}' && depth == 0){
                        dpos = pos;
                        break;
                    } else if (data[pos] == '}' && depth > 0){
                        depth--;
                    } else if (data[pos] == '{'){
                        depth++;
                    }
                }
                if (io == dpos){
                    printf("error, no closing bracket at line %d\n", i);
                    exit(1);
                } else {
                    i = dpos;
                    dpos = 0;
                }
            }
        } else if (data[i] == '\'') {
            if (multiply - 1 == 0){
                memset(strip, 0, size * sizeof(int));
            } else {
                for (int k = 0; k < multiply - 1 && ptr + k < size; k++) {
                    strip[ptr + k] = 0;
                }
            }
        } else if (data[i] == '"') {
            if (multiply - 1 == 0){
                memset(strip, dottp, size * sizeof(int));
            } else {
                for (int k = 0; k < multiply - 1 && ptr + k < size; k++) {
                    strip[ptr + k] = dottp;
                }
            }
        } else if (data[i]=='/'){
        	if (dottp==0){
        		nl_multiply=size-1;
        	} else if(dottp==1){
        		nl_multiply=i;
        	}
        }
        
        if (data[i] >= '0' && data[i] <= '9'){
            multiply += data[i] - '0';
        } else {
            multiply = 1;
            multiply += nl_multiply;
        }
        
        if (rbr){
            multiply = rbr_multiply;
            rbr = 0;
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

int *sbracket(int *strip, int location, int multiply, int length, char data[]){
    static int retd[2];
    int ret = 0;
    int dottp = 0;
    int mult = multiply;
    int nl_multiply=0;
    int val1 = 0;
    int val2 = 0;
    location++;
    int *nested;
    int mode = 0;
    int separator_hit = 0;
    
    while (location < length) {
        if (data[location] == ']') {
            break;
        } else if (data[location] == '(') {
            nested = rbracket(strip, location, mult, length, data);
            ret += nested[0];
            location = nested[1] + 1;
            continue;
        } else if (data[location] == '[') {
            nested = sbracket(strip, location, mult, length, data);
            ret += nested[0];
            location = nested[1] + 1;
            continue;
        } else if (data[location] == '|') {
            if (mult > 0 && mult <= BASE_STRIP) {
                nl_multiply = strip[mult-1];
            }
        } else if (data[location]=='?'){
            if (mult < PTR_MAX && mult > 0){
                dottp = mult-1;
            }
        } else if (data[location] == '+') {
            ret += mult;
        } else if (data[location] == '-') {
            ret -= mult;
        } else if (data[location] == ',') {
            mode = 0;
            val1 = ret;
            ret = 0;
            separator_hit = 1;
            location++;
            continue;
        } else if (data[location] == '.') {
            mode = 1;
            val1 = ret;
            ret = 0;
            separator_hit = 1;
            location++;
            continue;
        } else if (data[location] == ';') {
            mode = 2;
            val1 = ret;
            ret = 0;
            separator_hit = 1;
            location++;
            continue;
        } else if (data[location] == ':') {
            val2 = ret;
            ret = 0;
            location++;
            continue;
        }  else if (data[location]=='/'){
            if (dottp==0){
                nl_multiply=BASE_STRIP-1;
            } else if(dottp==1){
                nl_multiply=location;
            }
        } else if (data[location]=='\\'){
            nl_multiply=0;
        }
        if (data[location] >= '0' && data[location] <= '9'){
            mult += data[location] - '0';
        } else {
            mult = 1;
            mult += nl_multiply;
        }
        location++;
    }
    
    if (!separator_hit) {
        retd[0] = (ret != 0) ? 1 : 0;
    } else if (mode == 0) {
        retd[0] = (val1 == ret) ? 1 : 0;
    } else if (mode == 1) {
        retd[0] = (val1 != ret) ? 1 : 0;
    } else {
        retd[0] = (val1 == ret || val2 == ret) ? 1 : 0;
    }
    
    retd[1] = location;
    return retd;
}

int *rbracket(int *strip, int location, int multiply, int length, char data[]){
    static int retd[2];
    int dottp=0;
    int ret = 0;
    int mult = multiply;
    int nl_multiply=0;
    location++;
    int *nested;
    
    while (location < length) {
        if (data[location] == ')') {
            break;
        } else if (data[location] == '(') {
            nested = rbracket(strip, location, mult, length, data);
            ret += nested[0];
            location = nested[1] + 1;
            continue;
        } else if (data[location] == '|') {
            if (mult > 0 && mult <= BASE_STRIP) {
                nl_multiply = strip[mult-1];
            }
        } else if (data[location] == '+') {
            ret += mult;
        } else if (data[location]=='?'){
            if (mult < PTR_MAX && mult > 0){
                dottp = mult-1;
            }
        } else if (data[location] == '-') {
            ret -= mult;
      	} else if (data[location]=='/'){
            if (dottp==0){
                nl_multiply=BASE_STRIP-1;
            } else if(dottp==1){
                nl_multiply=location;
            }
        } else if (data[location]=='\\'){
            nl_multiply=0;
        }
        if (data[location] >= '0' && data[location] <= '9'){
            mult += data[location] - '0';
        } else {
            mult = 1;
            mult += nl_multiply;
        }
        location++;
    }
    
    retd[0] = ret;
    retd[1] = location;
    return retd;
}
