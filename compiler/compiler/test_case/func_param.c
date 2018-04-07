int succeed;
void E(int var, int val){
    if(var != val){
        printf("\n[error!] %d != %d\n", var, val);
        succeed = 0;
    }
}

void func11(int a){
    printf("func11, a = %d\n", a);
    E(a, 1);
}
void func12(char a){
    printf("func12, a = %c\n", a);
    E(a, 'a');
}

void func21(int a, int b){
    printf("func21, a = %d, b = %d\n", a, b);
    E(a, 1);
    E(b, 2);
}
void func22(char a, char b){
    printf("func22, a = %c, b = %c\n", a, b);
    E(a, 'a');
    E(b, 'b');
}
void func23(int a, char b){
    printf("func23, a = %d, b = %c\n", a, b);
    E(a, 1);
    E(b, 'b');
}
void func24(char a, int b){
    printf("func24, a = %c, b = %d\n", a, b);
    E(a, 'a');
    E(b, 2);
}

void func31(int a, int b, int c){
    printf("func31, a = %d, b = %d, c = %d\n", a, b, c);
    E(a, 1);
    E(b, 2);
    E(c, 3);
}
void func32(char a, char b, char c){
    printf("func32, a = %c, b = %c, c = %c\n", a, b, c);
    E(a, 'a');
    E(b, 'b');
    E(c, 'c');
}
void func33(int a, char b, int c){
    printf("func33, a = %d, b = %c, c = %d\n", a, b, c);
    E(a, 1);
    E(b, 'b');
    E(c, 3);
}
void func34(char a, int b, char c){
    printf("func34, a = %c, b = %d, c = %c\n", a, b, c);
    E(a, 'a');
    E(b, 2);
    E(c, 'c');
}

void func4(int a, int b, int c, int d){
    printf("func4, a = %d, b = %d, c = %d, d = %d\n", a, b, c, d);
    E(a, -1);
    E(b, -2);
    E(c, -3);
    E(d, -4);
}

void func51(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j){
    printf("func51, a = %d, b = %d, c = %d, d = %d, e = %d, ", a, b, c, d, e);
    printf("f = %d, g = %d, h = %d, i = %d, j = %d\n", f, g, h, i, j);
    E(a, -1);
    E(b, 2);
    E(c, -3);
    E(d, 4);
    E(e, -5);
    E(f, -1);
    E(g, -2);
    E(h, -3);
    E(i, -4);
    E(j, 0);
}
void func52(char a, char b, char c, char d, char e, char f, char g, char h, char i, char j){
    printf("func52, a = %c, b = %c, c = %c, d = %c, e = %c, ", a, b, c, d, e);
    printf("f = %c, g = %c, h = %c, i = %c, j = %c\n", f, g, h, i, j);
    E(a, 'a');
    E(b, 'b');
    E(c, 'c');
    E(d, 'd');
    E(e, 'e');
    E(f, 'f');
    E(g, 'g');
    E(h, 'h');
    E(i, 'i');
    E(j, 'j');
}
void func53(char a, int b, char c, int d, char e, int f, char g, int h ,char i, int j){
    printf("func53, a = %c, b = %d, c = %c, d = %d, e = %c, ", a, b, c, d, e);
    printf("f = %d, g = %c, h = %d, i = %c, j = %d\n", f, g, h, i, j);
    E(a, 'a');
    E(b, 2);
    E(c, 'c');
    E(d, 4);
    E(e, 'e');
    E(f, 6);
    E(g, 'g');
    E(h, 8);
    E(i, 'i');
    E(j, 10);
}

int main(){
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int g;
    int h;
    int i;
    int j;
    
    char ca;
    char cb;
    char cc;
    char cd;
    char ce;
    char cf;
    char cg;
    char ch;
    char ci;
    char cj;

    a = -1;
    b = 2;
    c = -3;
    d = 4;
    e = -5;
    f = a;
    g = -b;
    h = c;
    i = -d;
    //j = 0;

    ca = 'a';
    cb = 'b';
    cc = 'c';
    cd = 'd';
    ce = 'e';
    cf = 'f';
    cg = 'g';
    ch = 'h';
    ci = 'i';
    cj = 'j';

    succeed = 1;

    func11(1);
    func12('a');
    func21(1, 2);
    func22('a', 'b');
    func23(1, 'b');
    func24('a', 2);
    func31(1, 2, 3);
    func32('a', 'b', 'c');
    func33(1, 'b', 3);
    func34(ca, 2, cc);
    func4(-1, -2, -3, -4);
    func51(a, b, c, d, e, f, g, h, i, j);
    func52(ca, cb, cc, cd, ce, cf, cg, ch, ci, cj);
    func53('a', 2, 'c', 4, 'e', 6, 'g', 8, 'i', 10);
    if(succeed){
        printf("test [func_call] succeed!\n");
    }
    else{
        printf("test [func_call] faild!\n");
    }
    return 0;
}
