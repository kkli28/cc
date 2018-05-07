int succeed = 1;
int count = 0;
void EI(int var, int val){
    ++count;
    if(var != val){
        printf("%d [error] %d != %d\n", count, var, val);
        succeed = 0;
    }
}

void EC(char var, char val){
    ++count;
    if(var != val){
        printf("%d [error] %c != %c\n", count, var, val);
        succeed = 0;
    }
}

int max(int a, int b, int c){
    return a > b ? a > c ? a : b > c ? b : c : b > c ? b : a > c ? a : c;
}

int min(int a, int b, int c){
    return a < b ? a < c ? a : b < c ? b : c : b < c ? b : a < c ? a : c;
}

int main(){
    int a = 1;
    int b = 2;
    int c = 3;

    EI(a > b ? a : b, 2);
    EI(a > c ? a : c, 3);
    EI(b > c ? b : c, 3);

    EI(a < b ? a : b, 1);
    EI(a < c ? a : c, 1);
    EI(b < c ? b : c, 2);

    EI(a > b ? a > c ? a : b > c ? b : c : b > c ? b : a > c ? a : c, 3);
    EI(a < b ? a < c ? a : b < c ? b : c : b < c ? b : a < c ? a : c, 1);

    EI(max(a, b, c), c);
    EI(max(a, c, b), c);
    EI(max(b, a, c), c);
    EI(max(b, c, a), c);
    EI(max(c, a, b), c);
    EI(max(c, b, a), c);
    
    EI(min(a, b, c), a);
    EI(min(a, c, b), a);
    EI(min(b, a, c), a);
    EI(min(b, c, a), a);
    EI(min(c, a, b), a);
    EI(min(c, b, a), a);
    
    if(succeed){
        printf(">>>> succeed! <<<<\n");
    }
    else{
        printf("#### failed! ####\n");
    }
    return 0;
}