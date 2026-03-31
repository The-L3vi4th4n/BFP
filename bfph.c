#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>

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
int digit=0;
static int rot = 0;

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

#ifdef __APPLE__

#else
unsigned int arc4random_uniform(unsigned int x){
    static int seed=time(NULL);
    srand(seed);
    return(rand()%x);
}
#endif

int bt(int n, int i){
    int multiply=n;
    static int N=1;
    int result = 1;
    static int base = 13;
    while (n > 0){
        result += n % base;
        n /= base; 
    }
    base = ((base + result + N) / multiply) + multiply - i;
    N=n;
    return result-N;
}

int to_base(int n,int i,int multiply, int *strip) {
    int count=1000*multiply;
    count=(multiply|count)+count;
    unsigned int result = 0, place = 1;
    n++;
    static unsigned long N=1;
    n+=N;
    N ^= n * i * multiply;
    static unsigned long Nt=1;
    static int a=1;
    static int b=1;
    a=(((i+n)|Nt)^a)%BASE_STRIP;
    b=(((i-n)^Nt)|b)%BASE_STRIP;
    n+=strip[a];
    strip[a]-=n;
    n-=strip[b];
    strip[b]-=n;
    unsigned long o = n;
    unsigned long mix_i = ((unsigned long)i * 0x9e3779b97f4a7c15ULL) ^ (i << 3);
    unsigned long mix_m = ((unsigned long)multiply * 0x9e3779b97f4a7c15ULL) ^ (multiply >> 2);
    n=(mix_i*mix_m)^o;
    result -= o;
    while (n > 0 && count > 0) {
        result += (N-UINT_MAX) * place;
        N-=n/3;
        N+=Nt;
        Nt=N;
        n-=n/2;
        Nt+=(N << 2) | (N >> 3);
        Nt-=((Nt << 3) & (Nt >> 2));
        multiply+=((multiply << 3) & (multiply >> 2))+((unsigned int)(0)-Nt);
        place *= 10;
        count-=(N/n)+1;
    }
    Nt+=N & result;
    N+= Nt & n;
    return result%INT_MAX;
}

// Source for the function above - https://stackoverflow.com/a/912796
// Posted by anon, modified by community. See post 'Timeline' for change history
// Retrieved 2026-03-27, License - CC BY-SA 3.0

int *rbracket(int *strip, int location, int multiply, int length, char data[]);
int *sbracket(int *strip, int location, int multiply, int length, char data[]);

int main(int args, char *argv[]){
    srand(time(NULL));
    srand(time(NULL)*(rand()%1000));

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
        if (data[i] <= '0' && data[i] >= '9'){
            multiply=bt(multiply,i);
        }
        data[i] = (to_base(data[i],i,multiply,strip) + strip[ptr]) % 95 + rot; 
        if (data[i]==('<'-(unsigned char)(i)+(unsigned char)(multiply))){
            for (int k=0; k<multiply; k++){
                ptr--;      
                if (ptr<0){
                    ptr=size-1;
                }
            }
        } else if (data[i]==('>'-(unsigned char)(i)+(unsigned char)(multiply))){
            for (int k=0; k<multiply; k++){
                ptr++;
                if (ptr>=size){
                    ptr=0;
                }
            }
        } else if (data[i]==('+'-(unsigned char)(i)+(unsigned char)(multiply))){
            for (int k=0; k<multiply; k++){
                strip[ptr]++;
            }
        } else if (data[i]==('-'-(unsigned char)(i)+(unsigned char)(multiply))){
            for (int k=0; k<multiply; k++){
                strip[ptr]--;
            }
        } else if (data[i]==('.'-(unsigned char)(i)+(unsigned char)(multiply))){
            letter = (unsigned char)strip[ptr];
            for (int k=0; k<multiply; k++){
                printf("%c",(unsigned char)letter);
            }
        } else if (data[i]==(','-(unsigned char)(i)+(unsigned char)(multiply))){
            for (int k=0; k<multiply; k++){
                printf("%d",strip[ptr]);
            }
        } else if (data[i]==(':'-(unsigned char)(i)+(unsigned char)(multiply))){
            pvptr=ptr;
            ptr=0;
        } else if (data[i]==(';'-(unsigned char)(i)+(unsigned char)(multiply))){
            pvptr=ptr;
            ptr=size-1;
        } else if (data[i]==('~'-(unsigned char)(i)+(unsigned char)(multiply))){
            ptr=pvptr;
        } else if (data[i]==('_'-(unsigned char)(i)+(unsigned char)(multiply))){
            fflush(stdout);
        } else if (data[i]==('%'-(unsigned char)(i)+(unsigned char)(multiply))){
            for (int k=0; k<multiply; k++){
                puts("");
            }
        } else if (data[i]==('^'-(unsigned char)(i)+(unsigned char)(multiply))){
            if (multiply < PTR_MAX && multiply > 0){
                tpptr[multiply-1] = i;
            }
        } else if (data[i]==('*'-(unsigned char)(i)+(unsigned char)(multiply))){
            if (multiply < PTR_MAX && multiply > 0){
                i = tpptr[multiply-1];
                continue;
            }
        } else if (data[i]==('@'-(unsigned char)(i)+(unsigned char)(multiply))){
            usleep(multiply*1000);
        } else if (data[i]==('!'-(unsigned char)(i)+(unsigned char)(multiply))){
            printf("\033[2J\033[H");
            fflush(stdout);
        } else if (data[i]==('|'-(unsigned char)(i)+(unsigned char)(multiply))){
            if (multiply > 0 && multiply <= size) {
                nl_multiply = strip[multiply-1];
            }
        } else if (data[i]==('`'-(unsigned char)(i)+(unsigned char)(multiply))){
            break;
        } else if (data[i]==('\\'-(unsigned char)(i)+(unsigned char)(multiply))){
            nl_multiply = 0;
        } else if (data[i]==('?'-(unsigned char)(i)+(unsigned char)(multiply))){
            if (multiply < PTR_MAX && multiply > 0){
                dottp = multiply-1;
            }
        } else if (data[i]==('#'-(unsigned char)(i)+(unsigned char)(multiply))){
            if (dottp < PTR_MAX){
                lp[dottp]++;
                if (lp[dottp] < multiply){
                    i = tpptr[dottp];
                    continue;
                } else {
                    lp[dottp] = 0;
                }
            }
        } else if (data[i]==('$'-(unsigned char)(i)+(unsigned char)(multiply))){
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
        } else if (data[i]==('&'-(unsigned char)(i)+(unsigned char)(multiply))){
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
        } else if (data[i]==('='-(unsigned char)(i)+(unsigned char)(multiply))){
            range = multiply;
            if (range <= 0) range = 1;
            strip[ptr] = arc4random_uniform(range);
        } else if (data[i]=='('){
            br_o = rbracket(strip, i, multiply, size, data);
            rbr = 1;
            strip[ptr] = br_o[0];
            rbr_multiply = br_o[0];
            i = br_o[1];
        } else if (data[i]==('['-(unsigned char)(i)+(unsigned char)(multiply))){
            br_o = sbracket(strip, i, multiply, size, data);
            sbr = 1;
            strip[ptr] = br_o[0];
            sbr_multiply = br_o[0];
            i = br_o[1];
        } else if (data[i]==('{'-(unsigned char)(i)+(unsigned char)(multiply))){
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
        } else if (data[i] == ('\''-(unsigned char)(i)+(unsigned char)(multiply))) {
            if (multiply - 1 == 0){
                memset(strip, 0, size * sizeof(int));
            } else {
                for (int k = 0; k < multiply - 1 && ptr + k < size; k++) {
                    strip[ptr + k] = 0;
                }
            }
        } else if (data[i] == ('"'-(unsigned char)(i)+(unsigned char)(multiply))) {
            if (multiply - 1 == 0){
                memset(strip, dottp-1, size * sizeof(int));
            } else {
                for (int k = 0; k < multiply - 1 && ptr + k < size; k++) {
                    strip[ptr + k] = dottp-1;
                }
            }
        } else if (data[i]==('/'-(unsigned char)(i)+(unsigned char)(multiply))){
            if (dottp==1){
                nl_multiply=size-1;
            } else if(dottp==2){
                nl_multiply=i-1;
            }
        }
        
        int digit = to_base(data[i] - '0',i,multiply,strip);
        if (data[i] >= '0' && data[i] <= '9'){
            multiply += (digit-(data[i] - '0' - (unsigned char)(i)));
        } else {
            multiply = 1;
            multiply += (digit-nl_multiply);
        }
        
        if (rbr){
            multiply = rbr_multiply;
            rbr = 0;
        }
        rot = (rot + 1 + ((unsigned char)data[i]))*37 % 95;
        rot = (rot^0x9e3779b97f4a7c15ULL)*37 % 95;
        rot = ((rot >> 3) | (rot << 5))*37 % 95;
        i++;
    }
    
    fclose(f);
    free(strip);
    free(data);
    free(tpptr);
    return 0;
}

int *sbracket(int *strip, int location, int multiply, int length, char data[]){
    static int retd[2];
    int ret = 0;
    int mult = multiply;
    int val1 = 0;
    int val2 = 0;
    location++;
    int *nested;
    int mode = 0;
    int separator_hit = 0;
    
    while (location < length) {
        if (data[location] <= '0' && data[location] >= '9'){
            multiply=bt(mult+multiply,location);
        }
        data[location] = (to_base(data[location], location, mult+multiply, strip) + strip[mult%BASE_STRIP]) % 95 + rot;
        if (data[location] == (']'-(unsigned char)location-(unsigned char)(mult+multiply))) {
            break;
        } else if (data[location] == ('('-(unsigned char)location-(unsigned char)(mult+multiply))) {
            nested = rbracket(strip, location, mult, length, data);
            ret += nested[0];
            location = nested[1] + 1;
            continue;
        } else if (data[location] == ('['-(unsigned char)location-(unsigned char)(mult+multiply))) {
            nested = sbracket(strip, location, mult, length, data);
            ret += nested[0];
            location = nested[1] + 1;
            continue;
        } else if (data[location] == ('|'-(unsigned char)location-(unsigned char)(mult+multiply))) {
            if (mult > 0 && mult <= BASE_STRIP) {
                ret += strip[mult - 1];
            }
        } else if (data[location] == ('+'-(unsigned char)location-(unsigned char)(mult+multiply))) {
            ret += mult;
        } else if (data[location] == ('-'-(unsigned char)location-(unsigned char)(mult+multiply))) {
            ret -= mult;
        } else if (data[location] == (','-(unsigned char)location-(unsigned char)(mult+multiply))) {
            mode = 0;
            val1 = ret;
            ret = 0;
            separator_hit = 1;
            location++;
            continue;
        } else if (data[location] == ('.'-(unsigned char)location-(unsigned char)(mult+multiply))) {
            mode = 1;
            val1 = ret;
            ret = 0;
            separator_hit = 1;
            location++;
            continue;
        } else if (data[location] == (';'-(unsigned char)location-(unsigned char)(mult+multiply))) {
            mode = 2;
            val1 = ret;
            ret = 0;
            separator_hit = 1;
            location++;
            continue;
        } else if (data[location] == (':'-(unsigned char)location-(unsigned char)(mult+multiply))) {
            val2 = ret;
            ret = 0;
            location++;
            continue;
        }
        int digit = to_base(data[length] - '0',location,mult+multiply,strip) % 95 + 32;
        if (data[location] >= '0' && data[location] <= '9') {
            mult += (digit-data[location] - '0');
        } else {
            mult = digit;
        }
        if (location%2==0){
            rot = (rot + 1 + ((unsigned char)data[location]))*43 % 95;
            rot = (rot^0x9e3779b97f4a7c15ULL)*43 % 95;
            rot = ((rot >> 3) | (rot << 5))*43 % 95;
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
    int ret = 0;
    int mult = multiply;
    int nl_multiply=0;
    location++;
    int *nested;
    
    while (location < length) {
        if (data[location] <= '0' && data[location] >= '9'){
            multiply=bt(mult+multiply,location);
        }
        data[location] = (to_base(data[location], location, mult+multiply, strip) + strip[mult%BASE_STRIP]) % 95 + rot;
        if (data[location] == (')'-(unsigned char)location-(unsigned char)(mult+multiply))) {
            break;
        } else if (data[location] == ('('-(unsigned char)location-(unsigned char)(mult+multiply))) {
            nested = rbracket(strip, location, mult, length, data);
            ret += nested[0];
            location = nested[1] + 1;
            continue;
        } else if (data[location] == ('|'-(unsigned char)location-(unsigned char)(mult+multiply))) {
            if (mult > 0 && mult <= BASE_STRIP) {
                ret += strip[mult - 1];
            }
        } else if (data[location] == ('+'-(unsigned char)location-(unsigned char)(mult+multiply))) {
            ret += mult;
        } else if (data[location] == ('-'-(unsigned char)location-(unsigned char)(mult+multiply))) {
            ret -= mult;
        } else if (data[location]==('/'-(unsigned char)location-(unsigned char)(mult+multiply))) {
            nl_multiply=BASE_STRIP-1;
        }
        int digit = to_base(data[length] - '0',location,mult+multiply,strip) % 95 + 32;
        if (data[location] >= '0' && data[location] <= '9') {
            mult += (digit-data[location] - '0');
        } else {
            mult = digit;
        }
        if (nl_multiply!=0){
            mult=nl_multiply;
            nl_multiply=0;
        }
        if (location%3==0){
            rot = (rot + 1 + ((unsigned char)data[location]))*49 % 95;
            rot = (rot^0x9e3779b97f4a7c15ULL)*49 % 95;
            rot = ((rot >> 3) | (rot << 5))*49 % 95;
        }
        location++;
    }
    
    retd[0] = ret;
    retd[1] = location;
    return retd;
}
