int feb(int n){
    printf("\nfeb(%d)", n);
    if(n>1) return feb(n-1) + feb(n-2);
    else return n;
}

int main(){
    printf("the result of feb(10) is: %d", feb(10));
    return 1;
}
