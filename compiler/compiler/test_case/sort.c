int succeed = 1;
void E(int var, int val){
    //printf("var = %d, val = %d\n", var, val);
    if(var != val){
        printf("[error] %d != %d\n", var, val);
        succeed = 0;
    }
}

int arr[10] = {1,9,2,8,3,7,4,6,10,5};
void sort(int* arr, int len){
    int index;
    int i;
    int temp;
    index = 1;
    i = 0;
    while(index < len){
        i = index;
        while(i > 0 && arr[i] < arr[i-1]){
            temp = arr[i];
            arr[i] = arr[i-1];
            arr[i-1] = temp;
            --i;
		}
		++index;
    }
}

int main(){
    sort(arr, 10);
    E(arr[0], 1);
    E(arr[1], 2);
    E(arr[2], 3);
    E(arr[3], 4);
    E(arr[4], 5);
    E(arr[5], 6);
    E(arr[6], 7);
    E(arr[7], 8);
    E(arr[8], 9);
    E(arr[9], 10);
    if(succeed){
        printf("test [sort] succeed!\n");
    }
    else{
        printf("test [sort] failed!\n");
    }
    return 0;
}
