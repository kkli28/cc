int succeed = 1;
int count = 0;
void E(int var, int val){
    ++count;
    if(var != val){
        printf("\n%d [error] %d != %d\n", count, var, val);
        succeed = 0;
    }
}

enum {a, b=2, c, d=1, e, f, g=10000};
enum {h = -1, i = -2, j, k};
enum {};

int main() {
    succeed = 1;
    E(a, 0);
    E(b, 2);
    E(c, 3);
    E(d, 1);
    E(e, 2);
    E(f, 3);
    E(g, 10000);
    
    E(h, -1);
    E(i, -2);
    E(j, -1);
    E(k, 0);

    if(succeed){
        printf("test [enum] succeed!\n");
    }
    else{
        printf("test [enum] failed!\n");
    }
    return 0;
}
