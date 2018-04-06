int succeed;

void E(int var, int val){
    if(var != val){
        printf("======== error! =======\n");
        succeed = 0;
    }
}

int i;
char c;
int main(){
    i = 1;
    c = 2;
    E(i, 1);
    E(c, 2);
    return 0;    
}