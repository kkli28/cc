
enum { e1, e2, e3 = 100, e4, e5 = -1};
int arr[4] = {1, 2, 3, 4};
char* str = "abcd";

int sum(int* arr, int len){
    int result = 0;
    while(--len >= 0) { result = result + arr[len]; }
    return result;
}

void judgeEnum(){
    if(e1 != 0 || e2 != 1 || e3 != 100 || e4 != 101 || e5 != -1){
        printf("==== error! ====");
    }
}

void judgeArray(){
    if(sum(arr, 4) == 10) {}
    else{ printf("==== error! ===="); }
}

int main(){
    judgeEnum();
    judgeArray();
    printf("str = %s\n", str);
    return 0;
}

