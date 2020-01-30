#include <iostream>

using namespace std;

int main()
{
   int fil, col;
cout << "Dimesiones de la matriz: ";
cin >> fil >> col;

int matriz[fil][col];

for (int i = 1; i <= fil; i++){
    for (int j = 1; j <= col; j++){
            cout <<  rand()%10 << rand()%10<<"\t";

    }
    cout<<endl;
}
    return 0;
}
