template <class Mat>
MatrixStack<Mat>::MatrixStack(const Mat& base) : stack_() {
    stack_.push(base);
}

template <class Mat>
void MatrixStack<Mat>::push(const Mat& matrix) {
    stack_.push(stack_.top() * matrix);
}

template <class Mat>
void MatrixStack<Mat>::pop() {
    if (stack_.size() > 1) stack_.pop();
}

template <class Mat>
Mat& MatrixStack<Mat>::top() const {
    return stack_.top();
}

template <class Mat>
size_t MatrixStack<Mat>::size() const {
    return stack_.size();
}
