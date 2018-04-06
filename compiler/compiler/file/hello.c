int feb(int n){
    printf("\nfeb(%d)", n);
    if(n>1) return feb(n-1) + feb(n-2);
    else return n;
}

int func(){
    return 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 - 1 -2 -3-4-5-6-7-8;
}

int main(){
    printf("the result of feb(10) is: %d", feb(10));
    printf("the result of func() is: %d", func());
    return 1;
}
