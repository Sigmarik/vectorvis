#ifndef __IMPL_SELECTION_MASK_H
#define __IMPL_SELECTION_MASK_H

#include <cassert>

#include "Plug/Canvas/SelectionMask.h"

class SelectionMask : public plug::SelectionMask {
   public:
    SelectionMask();
    SelectionMask(const size_t width, const size_t height);
    SelectionMask(const size_t width, const size_t height,
                  const plug::SelectionMask &init_mask);

    void build(size_t width, size_t height);

    ~SelectionMask() { delete[] data_; }

    size_t getWidth(void) const override { return width_; }
    size_t getHeight(void) const override { return height_; }

    bool getPixel(size_t x, size_t y) const override;
    void setPixel(size_t x, size_t y, bool value) override;

    void fill(bool value) override;
    void invert(void) override;

   private:
    SelectionMask(const SelectionMask &mask) = default;
    SelectionMask &operator=(const SelectionMask &mask) = default;

    size_t width_, height_;
    bool *data_;
};

#endif