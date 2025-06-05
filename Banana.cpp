#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int BinNumber(int Number)
{
    if (Number == 0) return 0;
    int ones = 0;
    while (Number){
        if (Number % 2 == 1) ones++;
        Number /= 2;
    }
    return ones;
}
int processPair(pair<int, int> myPair){
    int ans = 0;
    ans += BinNumber(myPair.first);
    ans += BinNumber(myPair.second);
    return ans;
}
int main(){
    setlocale(LC_ALL, "rus");
    cout << "Введите N: ";
    int N;
    cin >> N;
    if (N < 0 || N > 264 - 1){
        cout << "ERROR";
        return 1;
    }
    pair<int, int> needCombo = { 0, N }; 
    int maxOnes = processPair(needCombo);
    for (int i = 1; i <= N / 2; i++){
        int n = N - i;
        pair<int, int> peremCombo = { i, n };
        int currentOnes = processPair(peremCombo);
        if (currentOnes > maxOnes){
            maxOnes = currentOnes;
            needCombo = { i, n };
        }
        else if (currentOnes == maxOnes){
            if (abs(n - i) > abs(needCombo.second - needCombo.first)){
                needCombo = { i, n };
            }
        }
    }
    cout << needCombo.first << " " << needCombo.second << endl;
    return 0;
}

