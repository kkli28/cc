int i = 0;
void move(int n, char from, char to) {
    printf("step %d: move %d from %c to %c\n", ++i, n, from, to);
}
void hanoi(int n, char from, char dp, char to) {
    if(n == 1) move(1, from, to);
    else{
        hanoi(n-1, from, to, dp);
        move(n, from, to);
        hanoi(n-1, dp, from, to);
    }
}
void main() {
    int n;
    scanf("%d", &n);
    hanoi(n, 'A', 'B', 'C');
    return 0;
}




