int succeed = 1;
int count = 0;
void EI(int var, int val){
    ++count;
    if(var != val){
        printf("%d [error] %d != %d\n", count, var, val);
        succeed = 0;
    }
}

void EC(char var, char val){
    ++count;
    if(var != val){
        printf("%d [error] %c != %c\n", count, var, val);
        succeed = 0;
    }
}

int arr1[4] = {1, 2, 3, 4};
char arr2[10] = {'a', 'b', 'c', 'd'};
int arr3[2];
char arr4[4];

int main(){
    EI(arr1[0], 1);
    EI(arr1[1], 2);
    EI(arr1[2], 3);
    EI(arr1[3], 4);
    EC(arr2[0], 'a');
    EC(arr2[1], 'b');
    EC(arr2[2], 'c');
    EC(arr2[3], 'd');
    EC(arr2[4], '\0');
    EI(arr3[0], 0);
    EI(arr3[1], 0);
    EC(arr4[0], '\0');
    EC(arr4[3], '\0');

    arr1[0]=4;
    arr1[3]=1;
    EI(arr1[0], 4);
    EI(arr1[3], 1);
    arr4[0]='A';
    arr4[1]='B';
    arr4[2]='C';
    arr4[3]='D';
    EC(arr4[0], 'A');
    EC(arr4[1], 'B');
    EC(arr4[2], 'C');
    EC(arr4[3], 'D');
    if(succeed){
        printf("test [array] succeed!\n");
    }
    else{
        printf("test [array] failed!\n");
    }
    return 0;
}
