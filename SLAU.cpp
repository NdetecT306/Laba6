#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <stdexcept>
using namespace std;
const int n = 4;
const double E = 0.001;
void forward(int n, double A[][4], double constants[]) {
    for (int i = 0; i < n; ++i) {
        int pivotRow = i;
        for (int k = i + 1; k < n; ++k) {
            if (abs(A[k][i]) > abs(A[pivotRow][i])) {
                pivotRow = k;
            }
        }
        if (abs(A[pivotRow][i]) < E) {
            cout << "Матрица вырождена. Решение невозможно." << endl;
            exit(1);
        }
        if (pivotRow != i) {
            for (int j = 0; j < n; ++j) {
                swap(A[i][j], A[pivotRow][j]);
            }
            swap(constants[i], constants[pivotRow]);
        }
        for (int j = i + 1; j < n; ++j) {
            double factor = A[j][i] / A[i][i];
            for (int k = i; k < n; ++k) {
                A[j][k] -= factor * A[i][k];
            }
            constants[j] -= factor * constants[i];
        }
    }
}
void solveUpperTriangular(int n, double A[][4], double b[], double x[]) {
    for (int i = n - 1; i >= 0; i--) {
        x[i] = b[i];
        for (int j = i + 1; j < n; j++) {
            x[i] -= A[i][j] * x[j];
        }
        x[i] /= A[i][i];
    }
}
void backward(int n, double matrix[][4], double constants[], double solution[]) {
    for (int i = n - 1; i >= 0; --i) {
        solution[i] = constants[i];
        for (int j = i + 1; j < n; ++j) {
            solution[i] -= matrix[i][j] * solution[j];
        }
        solution[i] /= matrix[i][i];
    }
}
bool simpleIteration(int n, double A[][4], double B[], double X[], double tolerance, int max_iterations, double& matrixNorm) {
    vector<double> X_simple(n, 0.0);
    vector<double> X_new(n);
    bool diagonallyDominant = true;
    for (int i = 0; i < n; ++i) {
        double sum = 0;
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                sum += abs(A[i][j]);
            }
        }
        if (abs(A[i][i]) < sum) {
            diagonallyDominant = false;
            break;
        }
    }
    if (!diagonallyDominant) cout << "Матрица НЕ обладает свойством диагонального преобладания." << endl;
    else cout << "Матрица обладает диагональным преобладанием." << endl;
    double C[4][4];
    double D[4];
    for (int i = 0; i < n; ++i) {
        D[i] = B[i] / A[i][i];
        for (int j = 0; j < n; ++j) {
            if (i == j) {
                C[i][j] = 0.0;
            }
            else {
                C[i][j] = -A[i][j] / A[i][i];
            }
        }
    }
    matrixNorm = 0.0;
    for (int i = 0; i < n; ++i) {
        double rowSum = 0.0;
        for (int j = 0; j < n; ++j) {
            rowSum += abs(C[i][j]);
        }
        matrixNorm = max(matrixNorm, rowSum);
    }
    cout << "Норма матрицы C: " << fixed << setprecision(3) << matrixNorm << endl;
    if (matrixNorm >= 1.0) {
        cout << "Достаточное условие сходимости не выполнено." << endl;
        return false; 
    }
    cout << "Достаточное условие сходимости выполнено (норма матрицы C < 1)." << endl;
    for (int iteration = 0; iteration < max_iterations; ++iteration) {
        for (int i = 0; i < n; ++i) {
            double sum = 0.0;
            for (int j = 0; j < n; ++j) {
                sum += C[i][j] * X_simple[j];
            }
            X_new[i] = sum + D[i];
        }
        double max_diff = 0.0;
        for (int i = 0; i < n; ++i) {
            max_diff = max(max_diff, abs(X_new[i] - X_simple[i]));
        }
        if (max_diff < tolerance) {
            cout << "Решение методом простых итераций найдено за " << iteration + 1 << " итераций." << endl;
            for (int i = 0; i < n; ++i) {
                X[i] = X_new[i];
                cout << "x" << i + 1 << " = " << fixed << setprecision(3) << X[i] << endl;
            }
            return true;
        }
        X_simple = X_new;
    }
    cout << "Решение не найдено за " << max_iterations << " итераций." << endl;
    return false;
}
bool simpleIteration(int n, double A[][4], double B[], double X[], double tolerance, int max_iterations = 100) {
    double dummyMatrixNorm; 
    return simpleIteration(n, A, B, X, tolerance, max_iterations, dummyMatrixNorm);
}
int main() {
    setlocale(LC_ALL, "rus");
    double A[n][n] = {
        {0.91,      -0.04, 0.21,     -18},
        {0.25,   -1.23, -0.23,      -0.09},
        {-0.21,   -0.23,     0.8,    -0.13},
        {0.15,   -1.31, 0.06,     -1.04}
    };
    double B[n] = {
        -1.24,
        -1.04,
        2.56,
        0.91
    };
    double X[n];
    cout << "Решение методом Гаусса:" << endl;
    double A_gauss[n][n];
    double B_gauss[n];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            A_gauss[i][j] = A[i][j];
        }
        B_gauss[i] = B[i];
    }
    forward(n, A_gauss, B_gauss);
    double X_forward[n];
    solveUpperTriangular(n, A_gauss, B_gauss, X_forward);
    cout << "Решение после прямого хода:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "x" << i + 1 << " = " << fixed << setprecision(3) << X_forward[i] << endl;
    }
    cout << endl;
    double X_gauss[n];
    backward(n, A_gauss, B_gauss, X_gauss);
    cout << "Решение после обратного хода:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "x" << i + 1 << " = " << fixed << setprecision(3) << X_gauss[i] << endl;
    }
    cout << endl;
    cout << "Решение методом простых итераций:" << endl;
    double matrixNorm = 0;
    if (!simpleIteration(n, A, B, X, E, 100, matrixNorm)) {
        cout << "Метод простой итерации не сошелся. Нельзя преобразовать к каноническому виду." << endl;
    }
    return 0;
}
