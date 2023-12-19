/**
 * @file curves.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Curves filter
 * @version 0.1
 * @date 2023-12-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __PLUGIN_CURVES_H
#define __PLUGIN_CURVES_H

#include "Plug/Filter.h"
#include "widgets/line_edit.h"

struct CurvesFilterData : public plug::PluginData {
    const char *getName(void) const override { return "Curves"; }

    const char *getTexturePath(void) const override { return nullptr; }
};

struct CurvesFilter : public plug::Filter {
    CurvesFilter() = default;

    plug::Plugin *tryGetInterface(size_t interface_id) override;

    void addReference(void) override {}

    void release(void) override {}

    const plug::PluginData *getPluginData(void) const override {
        return &data_;
    }

    void applyFilter(plug::Canvas &canvas) const override;

    plug::Widget *getWidget(void) override { return &interface_; }

   private:
    static CurvesFilter instance_;
    static CurvesFilterData data_;

    static LineEdit interface_;
};

extern "C" plug::Plugin *loadPlugin(void);

#endif
