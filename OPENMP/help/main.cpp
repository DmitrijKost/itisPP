#include<iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main() {
        int size;
        //Вывод на экран
        cout << "Ввод размера матрицы:" << endl;
        //Ввод с клавиатуры
        cin >> size;
        //Создание массива массивов(двумерная матрица)
        int **matrix = new int*[size];
        for(int i = 0; i < size; i++){
            matrix[i] = new int[size];
            for(int j=0; j < size; j++){
                //рандомное заполнение
                //matrix[i][j] = rand()%100;
                //симметричное заполнение
                matrix[i][j] = i+j;
                //Вывод матрицы
                printf("a[%d][%d] = %d\t",i,j,matrix[i][j]);
            }
            cout << endl;
        }
        //флаг имеет значение true до тех пор пока элементы симметричны
        int flag = true;
    //параллелим цикл
#pragma omp parallel for
        for(int i = 0; i < size; i++){
            //обновляем значение flag на всех потоках(если на одном стало false, то обновится до false везде)
#pragma omp flush(flag)
            //проверка, стоит ли проверять цикл дальше
            if(flag) {
                for (int j = i + 1; j < size; j++) {
                    //проверка на симметричнссть двух элемментов
                    if (matrix[i][j] != matrix[j][i]) {
                        //Если не симметрично, то обновляем значение flag и выходим из внутреннего цикла
                        flag = false;
                        #pragma omp flush(flag)
                        break;
                    }
                }
            }
        }
    //Вывод реультата(можно сделать и поаккуратнее, но мне нравятся подобные конструкии :3  )
    cout << "Матрица";
    if(!flag){
        cout << " не";
    }
    cout << " симметрична." << endl;
    return 0;
}