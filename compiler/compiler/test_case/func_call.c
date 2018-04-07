int succeed;
void E(int var, int val){
    if(var != val){
        printf("[error] %d != %d\n", var, val);
        succeed = 0;
    }
}

void func1(int a){
    printf("func1, a = %d\n", a);
    E(a, 1);
}

void func2(int a, int b){
    printf("func2, a = %d, b = %d\n", a, b);
    E(a, 1);
    E(b, 2);
}

void func3(int a, int b, int c){
    printf("func3, a = %d, b = %d, c = %d\n", a, b, c);
    E(a, 1);
    E(b, 2);
    E(c, 3);
}

void func4(int a, int b, int c, int d){
    printf("func4, a = %d, b = %d, c = %d, d = %d\n", a, b, c, d);
    E(a, -1);
    E(b, -2);
    E(c, -3);
    E(d, -4);
}

void func5(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j){
    printf("func5, a = %d, b = %d, c = %d, d = %d, e = %d, ", a, b, c, d, e);
    printf("f = %d, g = %d, h = %d, i = %d, j = %d\n", f, g, h, i, j);
    E(a, -1);
    E(b, 2);
    E(c, -3);
    E(d, 4);
    E(e, -5);
    E(f, -1);
    E(g, -2);
    E(h, -3);
    E(i, -4);
    E(j, 0);
}

int main(){
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int g;
    int h;
    int i;
    int j;
    a = -1;
    b = 2;
    c = -3;
    d = 4;
    e = -5;
    f = a;
    g = -b;
    h = c;
    i = -d;
    //j = 0;

    succeed = 1;

    func1(1);
    func2(1, 2);
    func3(1, 2, 3);
    func4(-1, -2, -3, -4);
    func5(a, b, c, d, e, f, g, h, i, j);
    if(succeed){
        printf("test [func_call] succeed!\n");
    }
    else{
        printf("test [func_call] faild!\n");
    }
    return 0;
}
