void func(){
    void* a = malloc(4);
    a[0] = 'a';
    a[1] = 'b';
    a[2] = 'c';
    a[3] = 'd';
    printf("%c %c %c %c\n", a[0], a[1], a[2], a[3]);

    int* b = malloc(16);
    b[0] = 1;
    b[1] = 2;
    b[2] = 3;
    b[3] = 4;
    printf("%d %d %d %d\n", b[0], b[1], b[2], b[3]);
}

int main(){
    void* a = malloc(4);
    a[0] = 'a';
    a[1] = 'b';
    a[2] = 'c';
    a[3] = 'd';
    printf("%c %c %c %c\n", a[0], a[1], a[2], a[3]);

    int* b = malloc(16);
    b[0] = 1;
    b[1] = 2;
    b[2] = 3;
    b[3] = 4;
    printf("%d %d %d %d\n", b[0], b[1], b[2], b[3]);
    func();
    return 0;
}