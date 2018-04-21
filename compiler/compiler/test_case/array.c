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
int arr3[4] = {-1, -2, 3, -4};
char arr4[4] = {-'A', 'B', 'C', 4};
char* arr5[2] = {"abcd", "ABCD"};
int arr6[2];
char arr7[4];

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

    EI(arr3[0], -1);
    EI(arr3[1], -2);
    EI(arr3[2], 3);
    EI(arr3[3], -4);
    EC(arr4[0], -'A');
    EC(arr4[1], 'B');
    EC(arr4[2], 'C');
    EC(arr4[3], 4);

    char a = (*arr5)[0];
    char b = (*arr5)[1];
    char c = (*arr5)[2];
    char d = (*arr5)[3];
    char a1 = arr5[0][0];
    char b1 = arr5[0][1];
    char c1 = arr5[0][2];
    char d1 = arr5[0][3];
    char a2 = arr5[1][0];
    char b2 = arr5[1][1];
    char c2 = arr5[1][2];
    char d2 = arr5[1][3];
    EC(a, 'a');
    EC(b, 'b');
    EC(c, 'c');
    EC(d, 'd');
    
    EC(a1, 'a');
    EC(b1, 'b');
    EC(c1, 'c');
    EC(d1, 'd');
    EC(a2, 'A');
    EC(b2, 'B');
    EC(c2, 'C');
    EC(d2, 'D');
    arr5[0] = "EFGH";
    arr5[1] = "IJKL";
    EC(arr5[0][0], 'E');
    EC(arr5[0][1], 'F');
    EC(arr5[0][2], 'G');
    EC(arr5[0][3], 'H');
    EC(arr5[1][0], 'I');
    EC(arr5[1][1], 'J');
    EC(arr5[1][2], 'K');
    EC(arr5[1][3], 'L');

    EI(arr6[0], 0);
    EI(arr6[1], 0);
    EC(arr7[0], '\0');
    EC(arr7[1], '\0');

    if(succeed){
        printf(">>>> succeed! <<<<\n");
    }
    else{
        printf("#### failed! ####\n");
    }
    return 0;
}
