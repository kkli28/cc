int succeed = 1;
void E(int var, int val){
    if(var != val){
        printf("\n[error!] %d != %d\n", var, val);
        succeed = 0;
    }
}

int main(){
    
    int a = 0;
    int expr = 0;
    if(expr) { a = 10; }
    else { a = 20; }
    E(a, 10);

    expr = 1;
    if(expr) { a = 100; }
    else { a = 200; }
    E(a, 100);

    if(succeed) printf(">>>> succeed! <<<<\n");
    else printf("#### failed! ####\n");
    return 0;
}
