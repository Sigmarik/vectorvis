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

    new (&buffer_[size_]) T(value);
    ++size_;
}

template <class T>
void Stack<T>::pop() {
    if (size_ == 0) return;

    --size_;

    if (size_ * 4 < capacity_ && capacity_ > 16) {
        relocate(capacity_ / 2);
    }

    buffer_[size_].~T();
}

template <class T>
T& Stack<T>::top() const {
    return buffer_[size_ - 1];
}

template <class T>
T& Stack<T>::operator[](size_t index) const {
    return buffer_[index];
}

template <class T>
size_t Stack<T>::size() const {
    return size_;
}

template <class T>
void Stack<T>::relocate(size_t new_capacity) {
    if (new_capacity < size_) return;
    if (new_capacity == 0) return;

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
