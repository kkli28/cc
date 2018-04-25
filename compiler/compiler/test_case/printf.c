void func1(int a, char ca, char* sa){
    printf("func1: %d %c %s\n", a, ca, sa);
}

void func2(int a, int b, char ca, char cb, char* sa, char* sb){
    printf("func2: %d %c %s ", a, ca, sa);
    printf("%d %c %s\n", b, cb, sb);
}

void func3(int a, int b, int c, char ca, char cb, char cc, char* sa, char sb, char* sc){
    printf("func3: %d %c %s ", a, ca, sa);
    printf("%d %c %s ", b, cb, sb);
    printf("%d %c %s\n", c, cc, sc);
}

void func4(int a, int b, int c, int d, char ca, char cb, char cc, char cd, char* sa, char* sb, char* sc, char* sd){
    printf("func4: %d %c %s ", a, ca, sa);
    printf("%d %c %s ", b, cb, sb);
    printf("%d %c %s ", c, cc, sc);
    printf("%d %c %s ", d, cd, sd);
}

void func5(int a, int b, int c, int d, int e, char ca, char cb, char cc, char cd, char ce, char* sa, char* sb, char* sc, char* sd, char* se){
    printf("func5: %d %c %s ", a, ca, sa);
    printf("%d %c %s ", b, cb, sb);
    printf("%d %c %s ", c, cc, sc);
    printf("%d %c %s ", d, cd, sd);
    printf("%d %c %s\n", e, ce, se);
}

void func(int a, char ca,  char* sa){
    printf("func: a = %d, ca = %c, sa = %s\n", a, ca, sa);
}

int main(){
    //int
    printf("%d\n", 1);
    printf("%d %d\n", 1, 2);
    printf("%d %d %d\n", 1, 2, 3);
    printf("%d %d %d %d\n", 1, 2, 3, 4);
    printf("%d %d %d %d %d\n", 1, 2, 3, 4, 5);
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    int e = 5;
    printf("%d\n", a);
    printf("%d %d\n", a, b);
    printf("%d %d %d\n", a, b, c);
    printf("%d %d %d %d\n", a, b, c, d);
    printf("%d %d %d %d %d\n", a, b, c, d, e);

    //char
    printf("%c\n", 'A');
    printf("%c%c\n", 'A', 'B');
    printf("%c%c%c\n", 'A', 'B', 'C');
    printf("%c%c%c%c\n", 'A', 'B', 'C', 'D');
    printf("%c%c%c%c%c\n", 'A', 'B', 'C', 'D', 'E');
    char ca = 'A';
    char cb = 'B';
    char cc = 'C';
    char cd = 'D';
    char ce = 'E';
    printf("%c\n", ca);
    printf("%c %c\n", ca, cb);
    printf("%c %c %c\n", ca, cb, cc);
    printf("%c %c %c %c\n", ca, cb, cc, cd);
    printf("%c %c %c %c %c\n", ca, cb, cc, cd, ce);
    
    //string
    printf("%s\n", "a");
    printf("%s %s\n", "a", "ab");
    printf("%s %s %s\n", "a", "ab", "abc");
    printf("%s %s %s %s\n", "a", "ab", "abc", "abcd");
    printf("%s %s %s %s %s\n", "a", "ab", "abc", "abcd", "abcde");
    char* sa = "a";
    char* sb = "ab";
    char* sc = "abc";
    char* sd = "abcd";
    char* se = "abcde";
    printf("%s\n", sa);
    printf("%s %s\n", sa, sb);
    printf("%s %s %s\n", sa, sb, sc);
    printf("%s %s %s %s\n", sa, sb, sc, sd);
    printf("%s %s %s %s %s\n", sa, sb, sc, sd, se);
    func1(a, ca, sa);
    func2(a, b, ca, cb, sa, sb);
    func3(a, b, c, ca, cb, cc, sa, sb, sc);
    func4(a, b, c, d, ca, cb, cc, cd, sa, sb, sc, sd);
    func5(a, b, c, d, e, ca, cb, cc, cd, ce, sa, sb, sc, sd, se);
    func(a, ca, sa);
    return 0;
}
