#include <iostream>
#include <omp.h>
#include <omp.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

void first(){
    int total, n,m, **a;
    cout<<"Enter n,m:\n";
    cin>>n>>m;
    a = new int*[n];
    for(int i = 0; i <n; i++){
        a[i] = new int[m];
    }
#pragma omp parallel for if(n*m>10000&&n>1)
    for(int i = 0; i < n; i++){
        for(int j =0; j < m; j++) {
            a[i][j] = i + j;
        }
    }
    total = 0;
#pragma omp parallel for if(n*m>10000&&n>1)
    for(int i = 0; i < n; i++) {
        int sum = 0;
        for (int j = 0; j < m; j++) {
            if (i % 2 == j % 2) {
                sum += a[i][j];
            }
        }
#pragma atomic
        total += sum;
        delete []a[i];
    }
    delete []a;
    cout<<"Total: "<< total<<endl;
};

void second() {
    int  n, *a;
    cout<<"Enter n:\n";
    cin>>n;
    vector<int> myV(2);
    a = new int[n];
    int max = -100;
    for(int i = 0; i < n; i++){
            a[i] = rand()%100-50;
            printf("a[%d] = %d\t",i,a[i]);
            if(i%50 == 0 && i>0){cout<<endl;}
    }
    cout<<endl;
    myV[0]= -50;
    myV[1]= 50;
#pragma omp parallel for if(n>1000)
        for(int i = 1; i<n;i++) {
            switch (a[i]%2) {
                case 0: if(a[i] > myV[0]){
                        #pragma omp critical(max)
                        if(a[i] > myV[0]){myV[0]=a[i];}
                    }break;
                default: if(a[i] < myV[1]){
                        #pragma omp critical(min)
                        if(a[i] < myV[1]){myV[1]=a[i];}
                    }
            }
        }
    delete []a;
    cout<<"min: "<<myV[1]<<endl;
    cout<<"max: "<<myV[0]<<endl;
}

void third() {
    omp_lock_t maxl,minl;
    omp_init_lock(&maxl);
    omp_init_lock(&minl);
    int  n, *a;
    cout<<"Enter n:\n";
    cin>>n;
    vector<int> myV(2);
    a = new int[n];
    int max = -100;
    for(int i = 0; i < n; i++){
        a[i] = rand()%100-50;
        printf("a[%d] = %d\t",i,a[i]);
        if(i%50 == 0 && i>0){cout<<endl;}
    }
    cout<<endl;
    myV[0]= -50;
    myV[1]= 50;
#pragma omp parallel for if(n>1000)
    for(int i = 1; i<n;i++) {
        switch (a[i]%2) {
            case 0: if(a[i] > myV[0]){
                    omp_set_lock(&maxl);
                    if(a[i] > myV[0]){myV[0]=a[i];
                        omp_unset_lock(&maxl);}
                }break;
            default: if(a[i] < myV[1]){
                    omp_set_lock(&minl);
                    if(a[i] < myV[1]){myV[1]=a[i];
                        omp_unset_lock(&minl);}
                }
        }
    }
    delete []a;
    omp_destroy_lock(&maxl);
    omp_destroy_lock(&minl);
    cout<<"min: "<<myV[1]<<endl;
    cout<<"max: "<<myV[0]<<endl;
}

void four() {
    int min, max, amount, mid, n, m, **a;
    cout << "Enter n,m:\n";
    cin >> n >> m;
    a = new int *[n];
    for (int i = 0; i < n; i++) {
        a[i] = new int[n];
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = rand() % 100 - 50;
            cout<<a[i][j]<<"\t";
        }
        cout<<endl;
    }
    min = a[0][0];
    max = a[0][0];
#pragma omp parallel sections
    {
#pragma omp section
        {
            int sum = 0;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    sum += a[i][j];
                }
            }
            mid = (sum / (m * n));
        }
#pragma omp section
        {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (a[i][j] > max) {
                        max = a[i][j];
                    }
                    if (a[i][j] < min) {
                        min = a[i][j];
                    }
                }
            }
        }
#pragma omp section
        {
            amount = 0;
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (a[i][j] % 3 == 0) {
                        amount += 1;
                    }
                }

            }
        }
    }
    printf("Mid : %d\nMin : %d\nMax : %d\nAmount elements division by 3 : %d\n",mid,min,max,amount);
    #pragma omp parallel for if(n > 1000)
    for(int i = 0; i < n; i++){
        delete []a[i];
    }
    delete []a;
}

int main() {
    while(true){
        cout<<"Выберите номер задания для продолжения:"<<endl;
        int next;
        cin>>next;
        switch(next){
            case 1:
                cout<<"Используя atomic. Задать матрицу размера n на m  и посчитать сумму матрицы(для четных строк чётные элементы, для нечётных строк нечётные)=ъъъъъъъъъъъъх"<<endl;
                first();break;
            case 2:
                cout<<"Используя critical. Использовать вектор. Для чётных максимум, для нечётных минимум"<<endl;
                second();break;
            case 3:
                cout<<"Используя lock. Использовать вектор. Для чётных максимум, для нечётных минимум"<<endl;
                third();break;
            case 4:
                cout<<"Sections. 2-мерная матрица 1)среденее арифметическое 2)максимальный и минимальный элемент матрицы 3) высчитать количество элементов кратных 3"<<endl;
                four();break;
            default:
                return 0;
        }
    }
}
