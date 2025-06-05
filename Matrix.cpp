#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <clocale> // Для setlocale
#include <sstream>
using namespace std;
int RandomNumbers(int start, int end) {
    int num;
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    ranlux24_base generation(seed);
    uniform_int_distribution<int> distribution(start, end);
    num = distribution(generation);
    return num;
}
int** Matrix(int m, int n, int start, int end) {
    int** a;
    a = new int* [m];
    for (int i = 0; i < m; i++) a[i] = new int[n]; // Исправлено: n, а не m
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n;j++) {
            a[i][j] = RandomNumbers(start, end);
        }
    }
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n;j++) cout << a[i][j] << " ";
        cout << endl;
    }
    return a;
}
vector<int> longestIncreasingSequence(int* a, int n) {
    vector<int> longest;
    vector<int> perem;
    if (n == 0) return longest;
    perem.push_back(a[0]);
    for (int i = 1; i < n; ++i) {
        if (a[i] > perem.back()) perem.push_back(a[i]);
        else {
            if (perem.size() > longest.size()) longest = perem;
            perem.clear();
            perem.push_back(a[i]);
        }
    }
    if (perem.size() > longest.size()) longest = perem;
    return longest;
}
bool isValid(int row, int col, int str3, int stb3) {
    return (row >= 0 && row < str3 && col >= 0 && col < stb3);
}

// Функция для рекурсивного поиска слова в матрице
bool findWord(char** matrix, const string& word, int row, int col, int wordIndex, int str3, int stb3, bool** visited) {
    // Если слово полностью найдено, возвращаем true
    if (wordIndex == word.length()) {
        return true;
    }

    // Если текущая позиция не валидна или символ не соответствует, возвращаем false
    if (!isValid(row, col, str3, stb3) || matrix[row][col] != word[wordIndex] || visited[row][col]) {
        return false;
    }

    // Помечаем текущую ячейку как посещенную
    visited[row][col] = true;

    // Рекурсивно ищем дальше во всех четырех направлениях
    bool found = findWord(matrix, word, row + 1, col, wordIndex + 1, str3, stb3, visited) ||
        findWord(matrix, word, row - 1, col, wordIndex + 1, str3, stb3, visited) ||
        findWord(matrix, word, row, col + 1, wordIndex + 1, str3, stb3, visited) ||
        findWord(matrix, word, row, col - 1, wordIndex + 1, str3, stb3, visited);

    // Если слово не найдено, снимаем метку посещения (backtracking)
    if (!found) {
        visited[row][col] = false;
    }

    return found;
}
int main()
{
    setlocale(LC_ALL, "rus");
    /*
    int m1, n1, start1 = -50, end1 = 50;
    cout << "Введите количество строк: ";
    cin >> m1;
    cout << "Введите количество столбцов: ";
    cin >> n1;
    int** a = Matrix(m1, n1, start1, end1); 
    vector<int> answer; 
    for (int i = 0; i < m1; ++i) {
        vector<int> rowLongestSeq = longestIncreasingSequence(a[i], n1);
        if (rowLongestSeq.size() >= answer.size()) {
            answer = rowLongestSeq;
        }
    }
    cout << "Самая длинная возрастающая последовательность:" << endl;
    if (answer.empty()) {
        cout << "В матрице нет возрастающих последовательностей." << endl;
    }
    else {
        for (int e : answer) {
            cout << e << " ";
        }
        cout << endl;
    }
    for (int i = 0; i < m1; ++i) {
        delete[] a[i];
    }
    delete[] a;
    int str2, stb2, start2 = 100, end2 = 200;
    cout << "Введите количество строк: ";
    cin >> str2;
    cout << "Введите количество столбцов: ";
    cin >> stb2;
    int** b = Matrix(str2, stb2, start2, end2); 
    vector<int> glavDiag; 
    double sumstr = 0, sumstb = 0, nstr = 0, nstb = 0;
    for (int i = 0; i < str2; i++) {
        for (int j = 0; j < stb2; j++) { 
            if (i % 2 == 0) { 
                sumstr += b[i][j];
                nstr++;
            }
            if (j % 2 == 1) {
                sumstb += b[i][j];
                nstb++;
            }
        }
    }
    double srstr = (nstr > 0) ? sumstr / nstr : 0;
    double srstb = (nstb > 0) ? sumstb / nstb : 0;
    for (int i = 0; i < str2 && i < stb2; ++i) {
        glavDiag.push_back(b[i][i]);
    }
    cout << "Главная диагональ: ";
    for (int val : glavDiag) {
        cout << val << " ";
    }
    cout << endl;
    if (srstb > srstr) {
        sort(glavDiag.begin(), glavDiag.end(), greater<int>()); 
    }
    else {
        sort(glavDiag.begin(), glavDiag.end()); 
    }
    cout << "Измененная главная диагональ: ";
    for (int val : glavDiag) {
        cout << val << " ";
    }
    cout << endl;
    for (int i = 0; i < str2; ++i) {
        delete[] b[i];
    }
    delete[] b;
    */
    int str3, stb3;
    cout << "Введите количество строк: ";
    cin >> str3;
    cout << "Введите количество столбцов: ";
    cin >> stb3;
    char** matrix = new char* [str3];
    for (int i = 0; i < str3; ++i) {
        matrix[i] = new char[stb3];
    }

    // Создаем матрицу для отслеживания посещенных ячеек
    bool** visited = new bool* [str3];
    for (int i = 0; i < str3; ++i) {
        visited[i] = new bool[stb3];
        for (int j = 0; j < stb3; ++j) {
            visited[i][j] = false; // Инициализируем все ячейки как непосещенные
        }
    }

    // Заполняем матрицу
    cout << "Введите элементы матрицы:" << endl;
    for (int i = 0; i < str3; ++i) {
        for (int j = 0; j < stb3; ++j) {
            cin >> matrix[i][j];
        }
    }
    cout << "Введенная матрица:" << endl;
    for (int i = 0; i < str3; ++i) {
        for (int j = 0; j < stb3; ++j) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
    // Вводим слово
    string word;
    cout << "Введите слово: ";
    cin >> word;

    // Проверяем длину слова
    if (word.length() > str3 * stb3) {
        cout << "false" << endl;
        // Освобождаем память, выделенную для матрицы
        for (int i = 0; i < str3; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;

        // Освобождаем память, выделенную для visited
        for (int i = 0; i < str3; ++i) {
            delete[] visited[i];
        }
        delete[] visited;
        return 0;
    }
    // Ищем слово в матрице
    bool found = false;
    for (int i = 0; i < str3; ++i) {
        for (int j = 0; j < stb3; ++j) {
            //  Сбрасываем состояние visited для каждой новой стартовой позиции
            for (int r = 0; r < str3; ++r) {
                for (int c = 0; c < stb3; ++c) {
                    visited[r][c] = false;
                }
            }
            if (findWord(matrix, word, i, j, 0, str3, stb3, visited)) {
                found = true;
                break; // Выходим из внутреннего цикла, если слово найдено
            }
        }
        if (found) {
            break; // Выходим из внешнего цикла, если слово найдено
        }
    }

    // Выводим результат
    cout << (found ? "true" : "false") << endl;
    return 0;
}
