/**
 * @file edge_detect.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Edge detection filter
 * @version 0.1
 * @date 2023-12-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __PLUGIN_EDGE_DETECT_H
#define __PLUGIN_EDGE_DETECT_H

#include "Plug/Filter.h"

struct EdgeFilterData : public plug::PluginData {
    const char *getName(void) const override { return "Edge detect"; }

    const char *getTexturePath(void) const override { return nullptr; }
};

struct EdgeFilter : public plug::Filter {
    EdgeFilter() = default;

    plug::Plugin *tryGetInterface(size_t interface_id) override;

    void addReference(void) override {}

    void release(void) override {}

    const plug::PluginData *getPluginData(void) const override {
        return &data_;
    }

    void applyFilter(plug::Canvas &canvas) const override;

    plug::Widget *getWidget(void) override { return nullptr; }

   private:
    static EdgeFilter instance_;
    static EdgeFilterData data_;
};

extern "C" plug::Plugin *loadPlugin(void);

#endif
