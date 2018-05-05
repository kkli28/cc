int func(int a, int b){
    int c = a + b;
    if(c > 0){
        int a = 100;
        printf("%d\n", a);
    }
    else{
        int a = 200;
        int b = 300;
        int c = a + b;
        printf("%d\n", c);
    }
}

int main(){
    func(1, 2);
}
