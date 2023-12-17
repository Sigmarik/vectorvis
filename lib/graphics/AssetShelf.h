/**
 * @file AssetShelf.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief AssetShelf data structure
 * @version 0.1
 * @date 2023-11-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __GRAPHICS_ASSET_SHELF_H
#define __GRAPHICS_ASSET_SHELF_H

#include <Plug/Graphics/Texture.h>

#include <SFML/Graphics/Font.hpp>

static const unsigned MARGIN_DEFAULT = 8;

struct DesignDescriptor {
    DesignDescriptor();

    void load(const char* path);

    unsigned margin = MARGIN_DEFAULT;
    plug::Texture image;
};

#define DESIGN(name, path) DSGN_##name,

enum DesignId {
#include "designs.hpp"
};

#undef DESIGN
#define DESIGN(name, path) +1
static const unsigned DESIGN_COUNT = 0
#include "designs.hpp"
    ;
#undef DESIGN

/**
 * @brief Asset storage
 *
 */
struct AssetShelf final {
    static const sf::Font& getFont();
    static DesignDescriptor& getDesign(DesignId id);

   private:
    AssetShelf();
    AssetShelf& getInstance();

    static sf::Font font_;
    static DesignDescriptor designs_[DESIGN_COUNT];

    static AssetShelf instance_;
};

#endif
