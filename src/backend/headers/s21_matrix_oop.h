#ifndef __S21MATRIX_H__
#define __S21MATRIX_H__

#include <cstring>
#include <iostream>

class S21Matrix {
 private:
  int rows_, cols_;
  double** matrix_;

  S21Matrix GetMinorMatrix(const S21Matrix& o, int row, int column);
  double Minor(const S21Matrix& o, int row, int column);

 public:
  S21Matrix();
  S21Matrix(int rows, int cols);
  S21Matrix(const S21Matrix& o);
  S21Matrix(S21Matrix&& o) noexcept;
  ~S21Matrix();

  S21Matrix& operator=(const S21Matrix& o);
  double& operator()(int row, int col);
  double& operator()(int row, int col) const;
  S21Matrix& operator+=(const S21Matrix& o);
  S21Matrix operator+(const S21Matrix& o);
  S21Matrix& operator-=(const S21Matrix& o);
  S21Matrix operator-(const S21Matrix& o);
  S21Matrix& operator*=(const S21Matrix& o);
  S21Matrix operator*(const S21Matrix& o);
  S21Matrix operator*(const S21Matrix& o) const;
  S21Matrix& operator*=(const double num);
  S21Matrix operator*(const double num);
  bool operator==(const S21Matrix& o) const;

  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  bool EqMatrix(const S21Matrix& other) const;
  void MulNumber(const double num);
  void MulMatrix(const S21Matrix& other);
  S21Matrix Transpose();
  S21Matrix CalcComplements();
  double Determinant();
  S21Matrix InverseMatrix();

  int GetRows() const;
  int GetCols() const;

  void SetRows(const int rows);
  void SetCols(const int rows);
};

#endif
