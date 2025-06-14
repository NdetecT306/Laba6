#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <stdexcept>
using namespace std;
const double E = 0.001;
void forward(int n, double** A, double* constants) {
    for (int i = 0; i < n; ++i) {
        int pivotRow = i;
        for (int k = i + 1; k < n; ++k) {
            if (abs(A[k][i]) > abs(A[pivotRow][i])) {
                pivotRow = k;
            }
        }
        if (abs(A[pivotRow][i]) < E) {
            cout << "Решение невозможно." << endl;
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
void solveUpperTriangular(int n, double** A, double* b, double* x) {
    for (int i = n - 1; i >= 0; i--) {
        x[i] = b[i];
        for (int j = i + 1; j < n; j++) {
            x[i] -= A[i][j] * x[j];
        }
        x[i] /= A[i][i];
    }
}
void backward(int n, double** matrix, double* constants, double* solution) {
    for (int i = n - 1; i >= 0; --i) {
        solution[i] = constants[i];
        for (int j = i + 1; j < n; ++j) {
            solution[i] -= matrix[i][j] * solution[j];
        }
        solution[i] /= matrix[i][i];
    }
}
bool simpleIteration(int n, double** A, double* B, double* X, double tolerance, int max_iterations, double& matrixNorm) {
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
    double** C = new double*[n];
    double* D = new double[n];
    for (int i = 0; i < n; ++i) {
        C[i] = new double[n];
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
    matrixNorm = 0;
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
        for (int i = 0; i < n; ++i) {
            delete[] C[i];
        }
        delete[] C;
        delete[] D;
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
            for (int i = 0; i < n; ++i) {
                delete[] C[i];
            }
            delete[] C;
            delete[] D;
            return true;
        }
        
        X_simple = X_new;
    }
    
    cout << "Решение не найдено за " << max_iterations << " итераций." << endl;
    for (int i = 0; i < n; ++i) {
        delete[] C[i];
    }
    delete[] C;
    delete[] D;
    return false;
}

int main() {
    setlocale(LC_ALL, "rus");
    int n = 4; 
    double** A = new double*[n];
    for (int i = 0; i < n; ++i) {
        A[i] = new double[n];
    }
    double* B = new double[n];
    double* X = new double[n];
    A[0][0] = 0.91; A[0][1] = -0.04; A[0][2] = 0.21; A[0][3] = -18;
    A[1][0] = 0.25; A[1][1] = -1.23; A[1][2] = -0.23; A[1][3] = -0.09;
    A[2][0] = -0.21; A[2][1] = -0.23; A[2][2] = 0.8; A[2][3] = -0.13;
    A[3][0] = 0.15; A[3][1] = -1.31; A[3][2] = 0.06; A[3][3] = -1.04;
    B[0] = -1.24; B[1] = -1.04; B[2] = 2.56; B[3] = 0.91;
    cout << "Решение методом Гаусса:" << endl;
    // Создаем копии для метода Гаусса
    double** A_gauss = new double*[n];
    for (int i = 0; i < n; ++i) {
        A_gauss[i] = new double[n];
        for (int j = 0; j < n; ++j) {
            A_gauss[i][j] = A[i][j];
        }
    }
    double* B_gauss = new double[n];
    for (int i = 0; i < n; ++i) {
        B_gauss[i] = B[i];
    }
    forward(n, A_gauss, B_gauss);
    double* X_forward = new double[n];
    solveUpperTriangular(n, A_gauss, B_gauss, X_forward);
    cout << "Решение после прямого хода:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "x" << i + 1 << " = " << fixed << setprecision(3) << X_forward[i] << endl;
    }
    cout << endl;
    double* X_gauss = new double[n];
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
    for (int i = 0; i < n; ++i) {
        delete[] A[i];
        delete[] A_gauss[i];
    }
    delete[] A;
    delete[] A_gauss;
    delete[] B;
    delete[] B_gauss;
    delete[] X;
    delete[] X_forward;
    delete[] X_gauss;
    return 0;
}
