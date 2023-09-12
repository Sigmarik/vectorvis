/**
 * @file easy_vector.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Eay to use, not-overengineered vector
 * @version 0.1
 * @date 2023-09-08
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef EASY_VECTOR_H
#define EASY_VECTOR_H

struct VectorBrick {
    explicit VectorBrick(double value) : value_(value), next_(nullptr) {}
    ~VectorBrick() {
        if (next_) next_->~VectorBrick();
    }

    VectorBrick& operator+(VectorBrick& brick) {
        if (next_) {
            *next_ + brick;
        } else {
            next_ = &brick;
        }

        return *this;
    }

    void operator+=(const VectorBrick& vector) {
        VectorBrick* alpha = this;
        const VectorBrick* beta = &vector;

        while (alpha != nullptr && beta != nullptr) {
            alpha->value_ += beta->value_;
            alpha = alpha->next_;
            beta = beta->next_;
        }
    }

    void operator-=(const VectorBrick& vector) {
        VectorBrick* alpha = this;
        const VectorBrick* beta = &vector;

        while (alpha != nullptr && beta != nullptr) {
            alpha->value_ -= beta->value_;
            alpha = alpha->next_;
            beta = beta->next_;
        }
    }

    void operator*=(double scalar) {
        for (VectorBrick* brick = this; brick != nullptr;
             brick = brick->next_) {
            brick->value_ *= scalar;
        }
    }

    void operator/=(double scalar) {
        for (VectorBrick* brick = this; brick != nullptr;
             brick = brick->next_) {
            brick->value_ /= scalar;
        }
    }

    double length2() {
        double answer = 0.0;
        for (VectorBrick* brick = this; brick != nullptr;
             brick = brick->next_) {
            answer += brick->value_ * brick->value_;
        }
        return answer;
    }

    unsigned length() {
        unsigned answer = 0;
        for (VectorBrick* brick = this; brick != nullptr;
             brick = brick->next_) {
            ++answer;
        }
        return answer;
    }

    double operator[](unsigned index) {
        VectorBrick* brick = this;
        for (unsigned id = 0; id < index; ++id) {
            brick = brick->next_;
        }
        return brick->value_;
    }

   private:
    double value_;
    VectorBrick* next_;
};

#endif
