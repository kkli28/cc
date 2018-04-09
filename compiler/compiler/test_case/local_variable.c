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
    a= 1;
    b = 2;
     c= -1;
     printf("a = %d\n", a);
     printf("b = %d\n", b);
     int d;
     printf("define d\n");
     d=-2;
     printf("c = %d\n", c);
     printf("d = %d\n", d);
}

int main(){
    func();
    printf("test [local variable] succeed!\n");
    return 0;
}