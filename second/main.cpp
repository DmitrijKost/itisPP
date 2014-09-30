#include <iostream>
#include <sstream>
#include <vector>
#include <omp.h>


using namespace std;

string dToS(double h){
    ostringstream ss;
    ss << h;
    return ss.str();
}

void printV(vector<int> myV){
    for(int i = 0; i<myV.size(); i++){
        cout<<"Thread num = "<<myV[i]<<", i="<<i<<endl;
    }
}

void integral(float a, float b, int n) {
    while(true) {
        cout << "a) методом левых прямоугольников\n"
                "b) методом правых прямоугольников\n"
                "c) методом центральных прямоугольников\n"
                "other - exit\n";
        char next;
        double h = (b - a) / n;
        cin >> next;
        cout << h << "*(";
        switch (next) {
            case 'a':
#pragma omp parallel
            {
                string result = "";
#pragma omp for
                for (int i = 0; i < n; i++) {
                    result += "f(" + dToS(a + i * h) + ")+";
                };
                cout << result;
            }
                break;
            case 'b':
#pragma omp parallel
            {
                string result = "";
#pragma omp for
                for (int i = 1; i <= n; i++) {
                    result += "f(" + dToS(a + i * h) + ")+";
                };
                cout << result;
            }
                break;
            case 'c':
#pragma omp parallel
            {
                string result = "";
#pragma omp for
                for (int i = 1; i <= n; i++) {
                    result += "f(" + dToS(a + (i - 0.5) * h) + ")+";
                };
                cout << result;
            }
                break;
                default:
                    cout << "0)\n";return;
        }
        cout << "0)\n";
    }

}

void schedule(int n) {
    while(true) {
        int next;
        vector<int> myV(n);
        cout << "1) schedule(static)\n"
                "2) schedule(static, 4)\n"
                "3) schedule(dynamic)\n"
                "4) schedule(dynamic, 3)\nother - exit\n";
        cin >> next;
        switch (next) {
            case 1:
#pragma omp parallel for schedule(static)
                for (int i = 0; i < n; i++) {
                    myV[i] = omp_get_thread_num();
                }
                printV(myV);
                break;
            case 2:

#pragma omp parallel for schedule(static, 4)
                for (int i = 0; i < n; i++) {
                    myV[i] = omp_get_thread_num();
                };
                printV(myV);
                break;
            case 3:

#pragma omp parallel for schedule(dynamic)
                for (int i = 0; i < n; i++) {
                    myV[i] = omp_get_thread_num();
                };
                printV(myV);
                break;
            case 4:

#pragma omp parallel for schedule(dynamic, 3)
                for (int i = 0; i < n; i++) {
                    myV[i] = omp_get_thread_num();
                };
                printV(myV);
                break;
                default:myV.clear();
                    return;
        }
    }
}

int main() {
    while(true){
        cout<<"1 - интегралы\n2 - schedule\nother - exit"<<endl;
        int next, n;
        cin>>next;
        switch(next){
            case 1:
                float a,b;
                cout<<"Вычисление определенного интеграла c помощью квадратурных формул с клавиатуры задается a и b и количество отрезков N\nВведите a,b,n:"<<endl;
                cin>>a>>b>>n; integral(a,b,n);break;
            case 2:
                cout<<"Используя управление распределением итераций цикла между потоками\n"
                        "используя schedule(<type>[, chunk])\n"
                        "в цикле в вектор записываем thread[i] = omp_get_thread_num();\n"
                        "а после выводим вне параллельной области в цикле эти значения то есть (Num thread = 1, i = 2)\n"
                        "и разобраться, где какие потоки\nВведите количество иттераций цикла: "<<endl;
                cin>>n;
                schedule(n);break;
            default:
                return 0;
        }
    }
}
