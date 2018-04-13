int succeed;
void E(int var, int val){
    if(var != val){
        printf("\n[error!] %d != %d\n", var, val);
        succeed = 0;
    }
}

void func(){
    int a;
    printf("define a\n");
    int b;
    printf("define b\n");
    int c;
    printf("define c\n");
    a = 1;
    printf("a = %d\n", a);
    b = 2;
    printf("b = %d\n", b);
    c= -1;
    printf("c = %d\n", c);
    printf("a + b + c = %d\n", a + b + c);
    int d;
    printf("define d\n");
    d=-2;
    printf("d = %d\n", d);
    printf("a + b + c + d = %d\n", a + b + c + d);
    printf("a - b - c - d = %d\n", a - b - c - d);
}

int main(){
    func();
    printf("test [local variable] succeed!\n");
    return 0;
}