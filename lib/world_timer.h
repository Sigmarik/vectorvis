/**
 * @file world_timer.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief World timer module
 * @version 0.1
 * @date 2023-11-07
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef WORLD_TIMER_H
#define WORLD_TIMER_H

#include <SFML/System/Clock.hpp>

struct WorldTimer final {
    /**
     * @brief Return time in microseconds
     *
     * @return unsigned long long
     */
    static unsigned long long get();
    static double get_delta();

   private:
    WorldTimer();
    WorldTimer& operator=(const WorldTimer& timer) = default;

    WorldTimer& get_instance() {
        static WorldTimer instance;
        return instance;
    }

    static sf::Clock clock_;
    static sf::Clock tick_clock_;
    ;
};

#endif
