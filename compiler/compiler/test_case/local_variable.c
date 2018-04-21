int succeed = 1;
int count = 0;

void EI(int var, int val){
    ++count;
    if(var != val){
        printf("\n%d [error!] %d != %d\n", count, var, val);
        succeed = 0;
    }
}

void EC(char var, char val){
    ++count;
    if(var != val){
        printf("\n%d [error!] %d != %d\n", count, var, val);
        succeed = 0;
    }
}

int main(){
    int a = 1;
    int b = 2;
    int c = a + b;
    int d = a - b + c;
    EI(a, 1);
    EI(b, 2);
    EI(c, 3);
    EI(d, 2);
    char ca = 'A';
    char cb = 'B';
    char cc = cb - ca;
    char cd = ca + cc;
    EC(ca, 'A');
    EC(cb, 'B');
    EC(cc, (char)1);
    EC(cd, 'B');
    int e = 1;
    EI(e, 1);
    if(succeed){
        printf("test [local variable] succeed!\n");
    }
    else{
        printf("test [local variable] failed!\n");
    }
    return 0;
}
