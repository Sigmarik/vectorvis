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
#include "src/graphics/renderable/ray_tracing.h"
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

static const PointLight LIGHTS[] = {(PointLight){
    .position = Vec3d(0.0, 5.0, 10.0), .color = Color(1.0, 1.0, 1.0) * 10.0}};

static const Sphere SPHERES[] = {
    Sphere(
        Vec3d(0.0, 0.0, 10.0), 2.0,
        (Material){
            .color = Vec3d(0.9, 0.9, 0.9), .roughness = 0.03, .specular = 1.0}),
    Sphere(Vec3d(-3.0, -3.0, 13.0), 2.0,
           (Material){.color = Vec3d(0.8, 0.8, 0.86),
                      .roughness = 0.01,
                      .specular = 1.0,
                      .mirror = true})};

static const Plane PLANES[] = {
    Plane(
        Vec3d(0.0, 6.0, 10.0), Vec3d(0.0, -1.0, 0.0),
        (Material){
            .color = Vec3d(1.0, 1.0, 1.0), .roughness = 0.03, .specular = 1.0}),
    Plane(
        Vec3d(0.0, -6.0, 10.0), Vec3d(0.0, 1.0, 0.0),
        (Material){
            .color = Vec3d(1.0, 1.0, 1.0), .roughness = 0.03, .specular = 1.0}),
    Plane(
        Vec3d(0.0, 0.0, 16.0), Vec3d(0.0, 0.0, -1.0),
        (Material){
            .color = Vec3d(1.0, 1.0, 1.0), .roughness = 0.03, .specular = 0.0}),
    Plane(
        Vec3d(-6.0, 0.0, 10.0), Vec3d(1.0, 0.0, 0.0),
        (Material){
            .color = Vec3d(1.0, 0.1, 0.1), .roughness = 0.03, .specular = 1.0}),
    Plane(
        Vec3d(6.0, 0.0, 10.0), Vec3d(-1.0, 0.0, 0.0),
        (Material){
            .color = Vec3d(0.1, 1.0, 0.1), .roughness = 0.03, .specular = 1.0}),
    Plane(
        Vec3d(0.0, 0.0, -1.0), Vec3d(0.0, 0.0, 1.0),
        (Material){
            .color = Vec3d(0.1, 0.1, 1.0), .roughness = 0.03, .specular = 1.0}),
};

#endif
