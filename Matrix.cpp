#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <tuple>

template <class T>
class Matrix {
public:
    Matrix(const std::vector<std::vector<T>>& new_data) :
        row_count_(new_data.size()) {
        if (row_count_ != 0) {
            data_.reserve(row_count_ * new_data.front().size());
            for (const auto& row : new_data) {
                for (const auto& elem : row) {
                    data_.push_back(elem);
                }
            }
        }
    }

    Matrix(size_t n, size_t m) :
        row_count_(n),
        data_(n * m) {}

    Matrix(const Matrix<T>& other) = default;

    const T* operator[](const size_t index) const {
        return &data_[index * get_col_count()];
    }

    T* operator[](const size_t index) {
        return const_cast<T*>((*const_cast<const Matrix<T>*>(this))[index]);
    }

    size_t get_row_count_() const {
        return row_count_;
    }
    size_t get_col_count() const {
        return data_.size() / row_count_;
    }

    const std::pair<size_t, size_t> size() const {
        return std::make_pair(row_count_, get_col_count());
    }

    Matrix<T>& operator +=(const Matrix& other) {
        for (size_t i = 0; i < data_.size(); ++i) {
            data_[i] += other.data_[i];
        }
        return *this;
    }

    template <class Scalar>
    Matrix<T>& operator *=(const Scalar& scalar) {
        for (auto& elem : data_) {
            elem *= scalar;
        }
        return *this;
    }

    Matrix<T>& transpose() {
        const auto col_count = get_col_count();
        Matrix<T> trans(col_count, row_count_);
        for (size_t i = 0; i < row_count_; ++i) {
            for (size_t j = 0; j < col_count; ++j) {
                trans[j][i] = (*this)[i][j];
            }
        }
        swap_(trans);
        return *this;
    }

    Matrix<T> transposed() const {
        Matrix<T> result(*this);
        return result.transpose();
    }

    Matrix<T>& operator *=(const Matrix<T>& other) {
        Matrix<T> result(row_count_, other.get_col_count());
        for (size_t i = 0; i < row_count_; ++i) {
            for (size_t k = 0; k < other.row_count_; ++k) {
                for (size_t j = 0; j < other.get_col_count(); ++j) {
                    result[i][j] += (*this)[i][k] * other[k][j];
                }
            }
        }
        swap_(result);
        return *this;
    }

    auto begin() const {
        return data_.cbegin();
    }

    auto begin() {
        return data_.begin();
    }

    auto end() const {
        return data_.cend();
    }

    auto end() {
        return data_.end();
    }

private:
    std::vector<T> data_;
    size_t row_count_ = 0;

    void swap_(Matrix<T>& other) {
        data_ = std::move(other.data_);
        std::swap(row_count_, other.row_count_);
    }
};

template <class T>
const Matrix<T> operator +(const Matrix<T>& left, const Matrix<T>& right) {
    Matrix<T> result(left);
    return result += right;
}

template <class T, class Scalar>
const Matrix<T> operator *(const Matrix<T>& left, const Scalar& right) {
    Matrix<T> result(left);
    return result *= right;
}

template <class T>
const Matrix<T> operator *(const Matrix<T>& left, const Matrix<T>& right) {
    Matrix<T> result(left);
    return result *= right;
}

template <class T>
std::ostream& operator <<(std::ostream& out, const Matrix<T>& a) {
    size_t n = a.get_row_count_(), m = a.get_col_count();
    for (size_t i = 0; i < n; ++i) {
        out << a[i][0];
        for (size_t j = 1; j < m; ++j) {
            out << '\t';
            out << a[i][j];
        }
        if (i + 1 != n)
            out << '\n';
    }
    return out;
}
