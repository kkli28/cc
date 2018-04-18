int succeed = 1;
void E(int var, int val){
    if(var != val){
        printf("\n[error!] %d != %d\n", var, val);
        succeed = 0;
    }
}

void func(){
    return 1;
}

int main(){
    int a = 1;
    int b = 2;
    int c = a + b;
    int d = a - b + c;
    E(a, 1);
    E(b, 2);
    E(c, 3);
    E(d, 2);
    char ca = 'A';
    char cb = 'B';
    char cc = cb - ca;
    char cd = ca + cc;
    E(ca, 'A');
    E(cb, 'B');
    E(cc, 1);
    E(cd, 'B');
    int e = func();
    E(e, 1);
    if(succeed){
        printf("test [local variable] succeed!\n");
    }
    else{
        printf("test [local variable] failed!\n");
    }
    return 0;
}
