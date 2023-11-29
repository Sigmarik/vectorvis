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

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>

#include "Plug/Event.h"
#include "Plug/Math.h"
#include "common_utils.h"

/**
 * @brief Print the label of the program
 *
 */
void print_label();

plug::Transform get_screen_transform(unsigned size_x, unsigned size_y);

plug::Event* parse_event(const sf::Event& event, sf::Window& window);

#endif
