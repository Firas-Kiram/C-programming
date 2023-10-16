#include <stdio.h>

int a[10],b[10],c[10], m,i,k;
void print(int n){
    int j;
    for (j=0;j<n;j++)
    printf("\n %3d %3d %3d", a[j],b[j],c[j]);
    printf("\n_________________\n");
}

void moving(int *x, int *y){ int k=0;
    int d;
    while (x[k]==0) k++;
    d=x[k]; x[k]=0;

    k=m-1;
    while (y[k]!=0) k--;
    y[k]=d;
}

void hanoi(int *a, int *b, int *c, int n){
 if (n==1){
    moving(a, b);
    print(m);
 }
 else
 {
    hanoi (a, c, b, n-1);
    moving (a, b);
    print(m);
    hanoi(c, b, a, n-1);
 }
}

int main()
{
    scanf("%d", &m);
    for (i=0;i<m; i++){
        a[i]=i+1; b[i]=c[i]=0;
    }
    print(m);
    hanoi(a,c,b,m);
}
