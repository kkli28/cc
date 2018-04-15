int arr[2] = {2, 1};
int func(int* ptr){
    printf("%d\n", ptr[0]);
    printf("%d\n", ptr[1]);
}

int main(){
    int* a;
    int b;
    b = 1;
    a = &b;
    printf("address of a: %d\n", &a);
    printf("a: %d\n", a);
    printf("*a: %d\n", *a);
    func(arr);
    return 0;
}
