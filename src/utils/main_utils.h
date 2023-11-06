/**
 * @file main_utils.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Utility functions used in the main module
 * @version 0.1
 * @date 2023-07-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MAIN_UTILS_H
#define MAIN_UTILS_H

#include "common_utils.h"
#include "geometry/matrix.h"
#include "src/graphics/renderable/gui.h"
#include "src/graphics/renderable/renderable.h"

/**
 * @brief Print the label of the program
 *
 */
void print_label();

/**
 * @brief Calculate screen matrix by resolution
 *
 * @param size_x
 * @param size_y
 * @return Mat33d
 */
Mat33d get_screen_matrix(unsigned size_x, unsigned size_y);

void construct_gui(Panel& panel);

#endif
