int succeed = 1;
void E(int var, int val){
    if(var != val){
        printf("\n[error!] %d != %d\n", var, val);
        succeed = 0;
    }
}

int add(int a, int b) { return a + b; }
int mul(int a, int b, int c) {
    int r = a * b * c; 
    return r;
}
void swap(int* a, int* b){
    int t = *a;
    *a = *b;
    *b = t;
}

int main() {

    E(add(1, 2), 3);
    E(mul(1, 2, 3), 6);
    int a = 1;
    int b = 2;
    swap(&a, &b);
    E(a, 2);
    E(b, 1);

    if(succeed) printf(">>>> succeed! <<<<\n");
    else printf("#### failed! ####\n");
    return 0;
}
