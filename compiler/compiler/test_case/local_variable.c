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

void func(int a, int b, char c, char d){
    EI(a, 1);
    EI(b, 2);
    EC(c, 'c');
    EC(d, 'd');
    while(a--) {
        int a = 11;
        int b = 12;
        EI(a, 11);
        EI(b, 12);
        char c = 'a';
        char d = 'b';
        EC(c, 'a');
        EC(d, 'b');
        if(a){
            int a = 21;
            int b = 22;
            EI(a, 21);
            EI(b, 22);
            char c = 'x';
            char d = 'y';
            EC(c, 'x');
            EC(d, 'y');
        }
        EI(a, 11);
        EI(b, 12);
        EC(c, 'a');
        EC(d, 'b');
        a = 3;
        b = 4;
        EI(a, 3);
        EI(b, 4);
    }
    EI(a, -1);
    EI(b, 2);
    EC(c, 'c');
    EC(d, 'd');
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

    if(a){
        int a = 11;
        int b = 12;
        int c = a + b;
        int d = a - b + c;
        char ca = 'a';
        char cb = 'b';
        char cc = cb - ca;
        char cd = ca + cc;
        EI(a, 11);
        EI(b, 12);
        EI(c, 23);
        EI(d, 22);
        EC(ca, 'a');
        EC(cb, 'b');
        EC(cc, char(1));
        EC(cd, 'b');
        if(a){
            char ca = 'x';
            char cb = 'y';
            char cc = cb - ca;
            char cd = ca + cc;
            int a = 21;
            int b = 22;
            int c = a + b;
            int d = a - b + c;
            EI(a, 21);
            EI(b, 22);
            EI(c, 43);
            EI(d, 42);
            EC(ca, 'x');
            EC(cb, 'y');
            EC(cc, char(1));
            EC(cd, 'y');
        }
        ++a;
        ++b;
        c = a + b;
        d = 1;
        EI(a, 12);
        EI(b, 13);
        EI(c, 25);
        EI(d, 1);
    }
    EI(a, 1);
    EI(b, 2);
    EI(c, 3);
    EI(d, 2);

    func(1, 2, 'c', 'd');

    if(succeed){
        printf(">>>> succeed! <<<<\n");
    }
    else{
        printf("#### failed! ####\n");
    }
    return 0;
}
