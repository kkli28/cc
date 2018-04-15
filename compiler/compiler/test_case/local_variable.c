int succeed;
void E(int var, int val){
    if(var != val){
        printf("\n[error!] %d != %d\n", var, val);
        succeed = 0;
    }
}

int main(){
    succeed = 1;
    int a;
    a = 1;
    int b;
    b = 2;
    int c;
    c= -1;
    int d;
    d=-2;
    E(a, 1);
    E(b, 2);
    E(c, -1);
    E(d, -2);
    if(succeed){
        printf("test [local variable] succeed!\n");
    }
    else{
        printf("test [local variable] failed!\n");
    }
    return 0;
}