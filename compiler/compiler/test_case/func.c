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

int main(){
}
