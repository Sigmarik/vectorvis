#include <stdio.h>
#include <stdlib.h>

template <class T>
Stack<T>::Stack()
    : buffer_((T*)calloc(16, sizeof(T))), size_(0), capacity_(16) {}

template <class T>
Stack<T>::Stack(const Stack<T>& stack) {
    size_ = stack.size_;
    capacity_ = stack.capacity_;
    buffer_ = (T*)calloc(capacity_, sizeof(T));

    for (size_t id = 0; id < size_; ++id) {
        buffer_[id] = stack.buffer_[id];
    }
}

template <class T>
Stack<T>::~Stack() {
    if (capacity_ == 0) return;

    for (size_t id = 0; id < size_; ++id) {
        buffer_[id].~T();
    }

    free(buffer_);
    buffer_ = NULL;
    size_ = 0;
    capacity_ = 0;
}

template <class T>
Stack<T>& Stack<T>::operator=(const Stack<T>& stack) {
    if (buffer_ != NULL) {
        for (size_t id = 0; id < size_; ++id) {
            buffer_[id].~T();
        }
        free(buffer_);
    }

    size_ = stack.size_;
    capacity_ = stack.capacity_;
    buffer_ = (T*)calloc(capacity_, sizeof(T));

    for (size_t id = 0; id < size_; ++id) {
        buffer_[id] = stack.buffer_[id];
    }

    return *this;
}

template <class T>
void Stack<T>::push(const T& value) {
    if (size_ >= capacity_) {
        relocate(capacity_ == 0 ? 1 : capacity_ * 2);
    }

    buffer_[size_] = value;
    ++size_;
}

template <class T>
void Stack<T>::pop() {
    if (size_ == 0) return;

    if (size_ - 1 < capacity_ / 4 && capacity_ >= 2) {
        relocate(capacity_ / 2);
    }

    buffer_[size_ - 1].~T();
    --size_;
}

template <class T>
T& Stack<T>::top() {
    return buffer_[size_ - 1];
}

template <class T>
T& Stack<T>::operator[](unsigned index) {
    return buffer_[index];
}

template <class T>
size_t Stack<T>::size() {
    return size_;
}

template <class T>
void Stack<T>::relocate(size_t new_capacity) {
    if (new_capacity < size_) return;

    T* new_buffer = (T*)calloc(new_capacity, sizeof(T));
    for (size_t id = 0; id < size_; ++id) {
        new_buffer[id] = buffer_[id];
    }

    for (size_t id = 0; id < size_; ++id) {
        buffer_[id].~T();
    }

    free(buffer_);

    buffer_ = new_buffer;
    capacity_ = new_capacity;
}
