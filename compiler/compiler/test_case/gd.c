int a = 1;
int b = 2, c = 3;
int d = 4, *e = 0xffffff, **f = 0x0000ff;
char g = 'a';
char h = 'a', i = 'b', *j = 0123456;

void func(){
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
}

int main(){
    func();
    printf("%d", a);
    return 0;
}
