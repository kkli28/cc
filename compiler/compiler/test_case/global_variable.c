int succeed = 1;
int count = 0;
void EI(int var, int val){
    ++count;
    if(var != val){
        printf("\n%d [error!] %d != %d\n", count, var, val);
        succeed = 0;
    }
}

void EC(char var, char val){
    ++count;
    if(var != val){
        printf("\n%d [error!] %d != %d\n", count, var, val);
        succeed = 0;
    }
}

void ECP(char* var, char* val){
    ++count;
    if(var != val){
        printf("\n%d [error!] %d != %d\n", count, var, val);
        succeed = 0;
    }
}

void EIP(int* var, int* val){
    ++count;
    if(var != val){
        printf("\n%d [error!] %d != %d\n", count, var, val);
        succeed = 0;
    }
}

void EIPP(int** var, int** val){
    ++count;
    if(var != val){
        printf("\n%d [error!] %d != %d\n", count, var, val);
        succeed = 0;
    }
}

int a = 1;
int b = 2, c = 3;
int d = 4, *e = 0xffffff, **f = 0x0000ff;
char g = 'a';
char h = 'a', i = 'b', *j = 0123456;

int main(){
    EI(a, 1);
    EI(b, 2);
    EI(c, 3);
    EI(d, 4);
    EIP(e, (int*)(0xffffff));
    EIPP(f, (int**)0x0000ff);
    EC(g, 'a');
    EC(h, 'a');
    EC(i, 'b');
    ECP(j, (char*)0123456);
    if(succeed){
        printf("test [global_variable] succeed!\n");
    }
    else{
        printf("test [global_variable] failed!\n");
    }
    return 0;
}
