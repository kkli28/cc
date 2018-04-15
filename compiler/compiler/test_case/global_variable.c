int succeed;
void E(int var, int val){
    if(var != val){
        printf("\n[error!] %d != %d\n", var, val);
        succeed = 0;
    }
}

int a = 1;
int b = 2, c = 3;
int d = 4, *e = 0xffffff, **f = 0x0000ff;
char g = 'a';
char h = 'a', i = 'b', *j = 0123456;

int main(){
    succeed = 1;
    E(a, 1);
    E(b, 2);
    E(c, 3);
    E(d, 4);
    E(e, 0xffffff);
    E(f, 0x0000ff);
    E(g, 'a');
    E(h, 'a');
    E(i, 'b');
    E(j, 0123456);
    if(succeed){
        printf("test [global_variable] succeed!\n");
    }
    else{
        printf("test [global_variable] failed!\n");
    }
    return 0;
}
