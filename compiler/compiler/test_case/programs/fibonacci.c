int succeed = 1;
void E(int var, int val){
    //printf("var = %d, val = %d\n", var, val);
    if(var != val){
        printf("[error] %d != %d\n", var, val);
        succeed = 0;
    }
}

int fibonacci(int n){
    if(n <= 2) return 1;
    else return fibonacci(n-1) + fibonacci(n-2);
}

int main(){
    printf("the result of fibonacci(10) is %d\n", fibonacci(10));
    return 0;
}

