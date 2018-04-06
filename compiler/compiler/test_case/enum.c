enum {a, b=2, c, d=1, e, f, g=10000};
enum {h = -1, i = -2, j, k};
enum {};

int succeed;

void E(int var, int val){
    if(var != val){
        printf("[error] %d != %d\n", var, val);
        succeed = 0;
    }
}

int main() {
    succeed = 1;
    E(1, a, 0);
    E(2, b, 2);
    E(3, c, 3);
    E(4, d, 1);
    E(5, e, 2);
    E(6, f, 3);
    E(7, g, 10000);
    
    E(8, h, -1);
    E(9, i, -2);
    E(10, j, -1);
    E(11, k, 0);
    printf("a = %d\n", a);
    printf("b = %d\n", b);
    printf("c = %d\n", c);
    printf("d = %d\n", d);
    printf("e = %d\n", e);
    printf("f = %d\n", f);
    printf("g = %d\n", g);
    printf("h = %d\n", h);
    printf("i = %d\n", i);
    printf("j = %d\n", j);
    printf("k = %d\n", k);

    if(succeed){
        printf("test [enum] succeed!\n");
    }
    else{
        printf("test [enum] failed!\n");
    }
    return 0;
}
