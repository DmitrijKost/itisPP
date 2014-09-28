#include<iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

void first(int n) {
    int *a = new int[n];
    int *b = new int[n];
    int *c = new int[n];
#pragma omp parallel for
    for(int i = 0; i < n; i++){
        a[i] = rand()%100;
        b[i] = rand()%100;
    }
#pragma omp parallel for
    for(int i = 0; i < n; i++){
        c[i] = a[i] * b[i];
        printf("a[%d]=%d\tb[%d]=%d\tc[%d]=%d\n",i,a[i],i,b[i],i,c[i]);
    }
    delete []a;
    delete []b;
    delete []c;
}

void second(int n, int m) {
    int **a = new int*[n];
    int **b = new int *[n];
    int **c = new int*[n];
#pragma omp parallel for
    for(int i = 0; i < n; i++){
        a[i] = new int[m];
        b[i] = new int[m];
        c[i] = new int[m];
        for(int j = 0; j < m; j++){
            a[i][j] = rand()%100;
            b[i][j] = rand()%100;
        }
    }
#pragma omp parallel for
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            c[i][j] = a[i][j] + b[i][j];
            printf("a[%d][%d]=%d\tb[%d][%d]=%d\tc[%d][%d]=%d\n",i,j,a[i][j],i,j,b[i][j],i,j,c[i][j]);
        }
        delete []a[i];
        delete []b[i];
        delete []c[i];
    }
    delete []a;
    delete []b;
    delete []c;
}

void third(int x,int y,int z) {
    int **a = new int*[x];
    int **b = new int*[y];
    int **c = new int*[x];
    int d = (x>y)?(x>z)?x:z:(y>z)?y:z;
#pragma omp parallel for
    for(int i=0; i<d; i++){
        if(i < x){
            a[i] = new int[y];
            c[i] = new int[z];
        }
        if(i < y){
            b[i] = new int[z];
        }
        for(int j=0; j<d; j++){
            if(i < x && j < y){
                a[i][j] = rand()%100;
            }
            if(i < y && j < z){
                b[i][j] = rand()%100;
            }
        }
    }
#pragma omp parallel for
    for(int i = 0; i<x; i++){
        for(int j = 0; j < z; j++){
            c[i][j] = 1;
            for(int k = 0; k < y; k++){
                printf("a[%d][%d]=%d\tb[%d][%d]=%d\n",i,k,a[i][k],k,j,b[k][j]);
                c[i][j] += a[i][k] * b[k][j];
            }
            printf("c[%d][%d]=%d\n",i,j,c[i][j]);
        }
        delete []a[i];
        delete []c[i];
    }
#pragma omp parallel for
    for (int i = 0; i < y; i++) delete b[i];
    delete []a;
    delete []b;
    delete []c;
}

void four(int n) {
    int *a = new int[n];
    int *b = new int[n];
    int sum = 0;
#pragma omp parallel for
    for(int i=0; i<n; i++){
        a[i] = rand()%100;
        b[i] = rand()%100;
        printf("a[%d]=%d\tb[%d]=%d\n",i,a[i],i,b[i]);
    }
#pragma omp parallel for reduction(+:sum)
    for(int i = 0; i<n; i++){
        sum += a[i] * b[i];
    }
    cout<<"Сумма ="<<sum<<endl;
    delete []a;
    delete []b;
}
char flag;
int main() {
    do{
        cout<<"Введите номер функции:"<< endl;
        int next, n, m, k;
        cin>>next;
        switch(next){
            case 1:
                cout<<"Есть два массива. Посчитать произведение этих массивов в третий."<<endl<<"Введите размерность массива"<<endl;
                cin>>n; first(n);break;
            case 2:
                cout<<"Матрицы. Сумма матриц. C=A+B. Размерность матриц (n,m)"<<endl<<"Введите n и m"<<endl;
                cin>>n>>m; second(n,m);break;
            case 3:
                cout<<"Матрицы. Произведение матриц. C=A*B. A(n,k), B(k,m), C(n,m)"<<endl<<"Введите n,k,m"<<endl;
                cin>>n>>k>>m;
                third(n,k,m);break;
            case 4:
                cout<<"Посчитать сумму произведений двух векторов a и b."<<endl<<"Введите размерность пространства"<<endl;
                cin>>n;
                four(n);break;
            default: cout<<"Ещё раз"<<endl;
        }
        cout<<"Чтобы продолжить нажмите \"C\""<< endl;
        cin>>flag;
    }while(flag == 'C');
    return 0;
}