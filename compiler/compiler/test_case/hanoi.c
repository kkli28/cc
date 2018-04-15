int i=0;
void move(int n,char from,char to){
    printf("step %d: move %d from %c to %c\n", ++i, n, from, to);
}

void hanoi(int n, char from, char depend_on, char to){
    if(n == 1) move(1, from, to);
    else{
        hanoi(n-1, from, to, depend_on);
        move(n, from, to);
        hanoi(n-1, depend_on, from, to);
    }
}

void main(){
    char A;
    char B;
    char C;
    A= 'A';
    B= 'B';
    C= 'C';
    hanoi(3, A, B, C);
    return 0;
}
