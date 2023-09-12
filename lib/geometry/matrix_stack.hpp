template <class Mat>
MatrixStack<Mat>::MatrixStack(Mat base) : stack_() {
    stack_.push(base);
}

template <class Mat>
void MatrixStack<Mat>::push(Mat matrix) {
    stack_.push(matrix * stack_.top());
}

template <class Mat>
void MatrixStack<Mat>::pop() {
    if (stack_.size() > 1) stack_.pop();
}

template <class Mat>
Mat& MatrixStack<Mat>::top() {
    return stack_.top();
}

template <class Mat>
size_t MatrixStack<Mat>::size() {
    return stack_.size();
}
