enum {a, b=2, c, d=1, e, f, g=10000};
enum {h = -1, i = -2, j, k};
enum {};

int succeed;
void E(int var, int val){
    printf("var = %d, val = %d", var, val);
    if(var != val){
        printf("[error] %d != %d\n", var, val);
        succeed = 0;
    }
}

int main() {
    succeed = 1;
    E(a, '\0');
    E(a, 0);
    E(b, 2);
    E(c, 3);
    E(d, 1);
    E(e, 2);
    E(f,)
    
    if(succeed){
        printf("test [enum] succeed!\n");
    }
    else{
        printf("test [enum] failed!\n");
    }
    return 0;
}
