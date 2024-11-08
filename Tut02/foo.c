#include<stdio.h>


#define MAX(x,y) ((x > y) ? x : y)

int max(int x, int y) {
    return x > y ? x : y;
}

int main() {
    int a = 4;
    int b = 3;

    
    printf("%d\n", MAX(a,b));
    printf("%d\n", MAX(a++,b));
}

