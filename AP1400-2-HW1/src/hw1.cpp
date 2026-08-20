#include "hw1.h"

namespace algebra {
    //using Matrix = std::vector<std::vector<double>>; in hw1.h
    Matrix zeros(size_t n, size_t m) {
        return Matrix(n, std::vector<double>(m, 0.0));
    }

    Matrix ones(size_t n, size_t m) {
        return Matrix(n, std::vector<double>(m, 1.0));
    }

    Matrix random(size_t n, size_t m, double min, double max) {
        if (min > max) {
            throw std::logic_error("Max must bigger than min!");
        }
        std::random_device rd;
        //rd is a seed.
        std::mt19937 gen(rd());
        //gen is a generating machine.
        std::uniform_real_distribution<double> dist(min, max);
        //this creates a distribution, which converrs those 
        //raw numbers into useful values.
        Matrix mx = zeros(n, m);
        for (size_t i = 0; i < n; i++) {
            for(size_t j = 0; j <m; j++) {
                mx[i][j] = dist(gen);
            }
        }
        return mx;
    }

    void show(const Matrix& matrix) {
        std::cout << std::fixed << std::setprecision(3);
        for (const auto& row : matrix) {
            for (const auto& elem: row) {
                std::cout << elem;
            }
            std::cout << '\n';
        }
    }

    Matrix multiply(const Matrix& matrix, double c) {
        Matrix result = matrix;
        for (auto& row : result) {
            for (auto& elem : row) {
                elem *= c;
            }
        }
        return result;
    }

    Matrix multiply(const Matrix& matrix1, const Matrix& matrix2) {
        if (matrix1.empty()) {
            if (matrix2.empty()) {
                return Matrix{};
            } else {
                throw std::logic_error("cannot multiply empty matrix with non-empty matrix");
            }
        } else if (matrix2.empty()){
            throw std::logic_error("cannot multiply empty matrix with non-empty matrix");
        }
        int r1, c1, r2, c2;
        r1 = matrix1.size();
        c1 = matrix1[0].size();
        r2 = matrix2.size();
        c2 = matrix2[0].size();

        if (c1 != r2) {
            throw std::logic_error("the column of matrix1 must be same to the row of matrix2");
        }
        Matrix result = zeros(r1, c2);
        for (int r = 0; r < r1; r++) {
            for (int c = 0; c < c2; c++) {
                for (int i = 0; i < c1; i++) {
                    result[r][c] += matrix1[r][i] * matrix2[i][c];
                }
            }
        }
        return result;
    }

    Matrix sum(const Matrix& matrix, double c) {
        Matrix result = matrix;
        for (auto& row : result) {
            for (auto& elem : row) {
                elem += c;
            }
        }
        return result;
    }

    Matrix sum(const Matrix& matrix1, const Matrix& matrix2) {
        if (matrix1.empty()) {
            if (matrix2.empty()) {
                return Matrix{};
            } else {
                throw std::logic_error("cannot add empty matrix together with non-empty matrix");
            }
        } else if (matrix2.empty()){
            throw std::logic_error("cannot add empty matrix together with non-empty matrix");
        }
        int r1, c1, r2, c2;
        r1 = matrix1.size();
        c1 = matrix1[0].size();
        r2 = matrix2.size();
        c2 = matrix2[0].size();
        if (r1 != r2 || c1 != c2) {
            throw std::logic_error("the shape of two matrixes must be the same");
        }
        Matrix result = zeros(r1, c1);
        for (int r = 0; r < r1; r++) {
            for (int c = 0; c < c1; c++) {
                result[r][c] = matrix1[r][c] + matrix2[r][c];
            }
        }
        return result;
    }

    Matrix transpose(const Matrix& matrix) {
        if (matrix.empty()) {
            return Matrix{};
        }
        int r = matrix.size();
        int c = matrix[0].size();
        Matrix result = zeros(c, r);
        for (int i = 0; i < c; i++) {
            for (int j = 0; j < r; j++) {
                result[i][j] = matrix[j][i];
            }
        }
        return result;
    }

    Matrix minor(const Matrix& matrix, size_t n, size_t m) {
        if (matrix.empty()) {
            throw std::logic_error("an empty matrix cannot has minor matrix");
        }
        int r, c;
        r = matrix.size();
        c = matrix[0].size();
        if (r == 1 || c == 1) {
            return Matrix{};
        }
        if (n >= r || m >= c) {
            throw std::logic_error("(n, m) cannot be out of the matrix");
        }
        Matrix result = zeros(r - 1, c - 1);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                result[i][j] = matrix[i][j];
            }
        }
        for (int i = n; i < r - 1; i++) {
            for (int j = 0; j < m; j++) {
                result[i][j] = matrix[i + 1][j];
            }
        }
        for (int i = 0; i < n; i++) {
            for (int j = m; j < c - 1; j++) {
                result[i][j] = matrix[i][j + 1];
            }
        }
        for (int i = n; i < r - 1; i++) {
            for (int j = m; j < c - 1; j++) {
                result[i][j] = matrix[i + 1][j + 1];
            }
        }
        return result;
    }

    double determinant(const Matrix& matrix) {
        if (matrix.empty()) {
            return 1.0;
        }
        int r, c;
        r = matrix.size();
        c = matrix[0].size();
        if (r != c) {
            throw std::logic_error("only square matrix have determination");
        }
        if (r == 1) {
            return matrix[0][0];
        }
        double result = 0;
        int sign = 1;
        for (int i = 0; i < r; i++) {
            result += sign * matrix[0][i] * determinant(minor(matrix, 0, i));
            sign *= -1;
        }
        return result;
    }

    Matrix inverse(const Matrix& matrix) {
        if (matrix.empty()) {
            return Matrix{};
        }
        int r, c;
        r = matrix.size();
        c = matrix[0].size();

        if (r != c) {
            throw std::logic_error("only square matrix has inverse");
        }
        double D = determinant(matrix);
        if (D == 0) {
            throw std::logic_error("matrix whose determinant is zero does not have inverse");
        }
        Matrix result = zeros(r, c);
        int sign_r = 1;
        for (int i = 0; i < r; i++) {
            int sign_c = 1;
            for (int j = 0; j < c; j++) {
                result[i][j] = sign_r * sign_c * determinant(minor(matrix, i, j)) / D;
                sign_c *= -1;
            }
            sign_r *= -1;
        }
        result = transpose(result);
        return result;
    }

    Matrix concatenate(const Matrix& matrix1, const Matrix& matrix2, int axis) {
        if (matrix1.empty()) {
            if (matrix2.empty()) {
                return Matrix{};
            } else {
                throw std::logic_error("we cannot concatenate an empty matrix with an normal matrix");
            }
        } else if (matrix2.empty()) {
            throw std::logic_error("we cannot concatenate an empty matrix with an normal matrix");
        }
        int r1, c1, r2, c2;
        r1 = matrix1.size();
        c1 = matrix1[0].size();
        r2 = matrix2.size();
        c2 = matrix2[0].size();
        if (axis == 0) {
            if (c1 != c2) {
                throw std::logic_error("number of colums must be same between matrix1 and matrix2");
            }
            Matrix result = zeros(r1 + r2, c1);
            for (int i = 0; i < r1; i++) {
                for (int j = 0; j < c1; j++) {
                    result[i][j] = matrix1[i][j];
                }
            }
            for(int i = 0; i < r2; i++) {
                for (int j = 0; j < c1; j++) {
                    result[r1 + i][j] = matrix2[i][j];
                }
            }
            return result;
        }
        else if (axis == 1) {
            if (r1 != r2)
            {
                throw std::logic_error("number of rows must be same between matrix1 and matrix2");
            }

            Matrix result = zeros(r1, c1 + c2);

            for (int i = 0; i < r1; i++)
            {
                for (int j = 0; j < c1; j++)
                {
                    result[i][j] = matrix1[i][j];
                }

                for (int j = 0; j < c2; j++)
                {
                    result[i][c1 + j] = matrix2[i][j];
                }
            }
            return result;
        }
        else {
            throw std::logic_error("axi must be 0 or 1");
        }
    }

    Matrix ero_swap(const Matrix& matrix, size_t r1, size_t r2) {
        if (matrix.empty()) {
            return Matrix{};
        }
        int r, c;
        r = matrix.size();
        c = matrix[0].size();
        if ((r1 < 0 || r1 >= r) || (r2 < 0 || r2 >= r)) {
            throw std::logic_error("row out of range");
        }
        Matrix result = matrix;
        result[r1] = matrix[r2];
        result[r2] = matrix[r1];
        return result;
    }
    Matrix ero_multiply(const Matrix& matrix, size_t r, double c) {
        if (matrix.empty()) {
            return Matrix{};
        }
        int r_matrix, c_matrix;
        r_matrix = matrix.size();
        c_matrix = matrix[0].size();
        if (r < 0 || r >= r_matrix) {
            throw std::logic_error("row out of range");
        }
        Matrix result = matrix;
        for (int i = 0; i < c_matrix; i++) {
            result[r][i] *= c;
        }
        return result;
    }

    Matrix ero_sum(const Matrix& matrix, size_t r1, double c, size_t r2) {
        if (matrix.empty()) {
            return Matrix{};
        }
        int row, column;
        row = matrix.size();
        column = matrix[0].size();
        if ((r1 < 0 || r1 >= row) || (r2 < 0 || r2 >= row)) {
            throw std::logic_error("row out of range");
        }
        Matrix result = matrix;
        for (int i = 0; i < column; i++) {
            result[r2][i] += result[r1][i] * c;
        }
        return result;
    }
    Matrix upper_triangular(const Matrix& matrix) {
        if (matrix.empty()) {
            return Matrix{};
        }
        int row = matrix.size();
        int column = matrix[0].size();
        if (row != column) {
            throw std::logic_error("only square matrix has upper_triangular");
        }
        Matrix result = matrix;
        for (int i = 0; i < column; i++) {
            if (result[i][i] == 0) {
                for (int k = i + 1; k < row; k++)
                {
                    if (result[k][i] != 0)
                    {
                        result = ero_swap(result, i, k);
                        break;
                    }
                }
            }
            if (result[i][i] == 0) {
                continue;
            }
            for (int j = i + 1; j < row; j++) {
                double c = -1 * result[j][i] / result[i][i];
                result = ero_sum(result, i, c, j);
            }
        }
        return result;
    }
}