int succeed;
void E(int var, int val){
    if(var != val){
        printf("\n[error!] %d != %d\n", var, val);
        succeed = 0;
    }
}

int a, b, c;
int d;
int e;
char h, i, j;
char k;
char l;
int main(){
    succeed = 1;
    a = 1;
    b = 2;
    c = 3;
    d = -1;
    e = -2;
    
    h = 'h';
    i = 'i';
    j = 'j';
    k = '\n';
    l = ' ';
    E(a, 1);
    E(b, 2);
    E(c, 3);
    E(d, -1);
    E(e, -2);
    E(h, 'h');
    E(i, 'i');
    E(j, 'j');
    E(k, '\n');
    E(l, ' ');
    if(succeed){
        printf("test [global_variable] succeed!\n");
    }
    else{
        printf("test [global_variable] failed!\n");
    }
    return 0;    
}
