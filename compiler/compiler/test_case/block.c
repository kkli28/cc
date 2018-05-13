int succeed = 1;
void E(int var, int val){
    if(var != val){
        printf("\n[error!] %d != %d\n", var, val);
        succeed = 0;
    }
}

int main(){
    
    int a = 1;
    {
        int a = 2;
        E(a, 2);
    }
    E(a, 1);
    
    if(succeed) printf(">>>> succeed! <<<<\n");
    else printf("#### failed! ####\n");
    return 0;
}
