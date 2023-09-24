/**
 * @file simulation.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Molecule simulation
 * @version 0.1
 * @date 2023-09-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include "data_structures/stack.h"
#include "geometry/vector.hpp"

struct GasVolume;

enum MoleculeType { MT_LIGHT, MT_HEAVY };

struct Molecule {
    Molecule(const Molecule& molecule) = default;
    explicit Molecule(const Vec2d& position, const Vec2d& velocity,
                      MoleculeType type);

    Molecule& operator=(const Molecule& molecule) = default;

    void tick(GasVolume& volume, double delta_time);
    void round();

    void set_position(const Vec2d& position) { position_ = position; }
    void set_velocity(const Vec2d& velocity) { velocity_ = velocity; }
    Vec2d get_position() { return position_; }
    Vec2d get_velocity() { return velocity_; }

    void set_mass(double mass) { mass_ = mass; }
    double get_mass() { return mass_; }

    void set_alive(bool alive) { alive_ = alive; }
    bool is_alive() const { return alive_; }

    void collide(Molecule& molecule);

    MoleculeType get_type() const { return type_; }

   protected:
    MoleculeType type_;
    bool alive_ = true;
    double mass_ = 1.0;

   private:
    Vec2d position_ = Vec2d(0.0, 0.0);
    Vec2d velocity_ = Vec2d(0.0, 0.0);
};

struct LightMolecule : public Molecule {
    LightMolecule();
    explicit LightMolecule(const Vec2d& position,
                           const Vec2d& velocity = Vec2d(0.0, 0.0));

    LightMolecule(const LightMolecule& molecule) = default;

    // LightMolecule& operator=(const LightMolecule& molecule) = default;
};

struct HeavyMolecule : public Molecule {
    HeavyMolecule();
    explicit HeavyMolecule(const Vec2d& position,
                           const Vec2d& velocity = Vec2d(0.0, 0.0));

    HeavyMolecule(const HeavyMolecule& molecule) = default;

    // HeavyMolecule& operator=(const HeavyMolecule& molecule) = default;
};

struct MoleculeStorage {
    MoleculeStorage() = default;

    void add_heavy(const HeavyMolecule& molecule) {
        heavy_molecules_.push(molecule);
    }
    void add_light(const LightMolecule& molecule) {
        light_molecules_.push(molecule);
    }

    Molecule& operator[](size_t index) const {
        return (index < light_count_)
                   ? (Molecule&)light_molecules_[index]
                   : (Molecule&)heavy_molecules_[index - light_count_];
    }

    void update_size() {
        light_count_ = light_molecules_.size();
        heavy_count_ = heavy_molecules_.size();
    }

    size_t size() const { return light_count_ + heavy_count_; }

    void filter();

   private:
    size_t light_count_ = 0;
    size_t heavy_count_ = 0;
    Stack<LightMolecule> light_molecules_;
    Stack<HeavyMolecule> heavy_molecules_;
};

static const size_t MAX_CELL_CAPACITY = 20;
static const size_t GRID_RESOLUTION = 64;

struct GasVolume {
    GasVolume() = default;

    void add_light_molecule(const LightMolecule& molecule) {
        molecules_.add_light(molecule);
        molecules_.update_size();
    }
    void add_heavy_molecule(const HeavyMolecule& molecule) {
        molecules_.add_heavy(molecule);
        molecules_.update_size();
    }

    void tick(double delta_time);

    double get_temperature() const { return temperature_; }
    double get_pressure() const { return pressure_; }

    bool get_in_valve() const { return valve_in_open_; }
    bool get_out_valve() const { return valve_out_open_; }

    void set_in_value(bool state) { valve_in_open_ = state; }
    void set_out_value(bool state) { valve_out_open_ = state; }

    size_t molecule_count() const { return molecules_.size(); }
    Molecule& get_molecule(size_t id) const { return molecules_[id]; }

   private:
    void react(Molecule& alpha, Molecule& beta);

    void update_stats();

    Molecule** find_cell(Vec2d position) {
        int x_coord = (int)floor(position.get_x() * (double)GRID_RESOLUTION);

        if (x_coord < 0) x_coord = 0;
        if (x_coord >= GRID_RESOLUTION) x_coord = GRID_RESOLUTION - 1;

        int y_coord = (int)floor(position.get_y() * (double)GRID_RESOLUTION);

        if (y_coord < 0) y_coord = 0;
        if (y_coord >= GRID_RESOLUTION) y_coord = GRID_RESOLUTION - 1;

        return grid_[x_coord][y_coord];
    }

    void unpin(Molecule& molecule) {
        molecule.round();
        Molecule** cell = find_cell(molecule.get_position());

        unsigned left = 0;
        unsigned right = MAX_CELL_CAPACITY;
        while (right - left > 1) {
            unsigned mid = (left + right) / 2;
            if (cell[mid] != NULL)
                left = mid;
            else
                right = mid;
        }

        for (unsigned id = 0; id < right; ++id) {
            if (&molecule == cell[id]) {
                cell[id] = cell[left];
                cell[left] = NULL;
                return;
            }
        }
    }

    void pin(Molecule& molecule) {
        molecule.round();
        Molecule** cell = find_cell(molecule.get_position());

        unsigned left = 0;
        unsigned right = MAX_CELL_CAPACITY;
        while (right - left > 1) {
            unsigned mid = (left + right) / 2;
            if (cell[mid] != NULL)
                left = mid;
            else
                right = mid;
        }

        if (left < MAX_CELL_CAPACITY - 1) {
            cell[left + 1] = &molecule;
        }
    }

    void process_collisions(Molecule& molecule);

    bool valve_in_open_ = false;
    bool valve_out_open_ = false;

    MoleculeStorage molecules_;
    double energy_ = 0.0;
    double temperature_ = 0.0;
    double pressure_ = 0.0;

    Molecule* grid_[GRID_RESOLUTION][GRID_RESOLUTION][MAX_CELL_CAPACITY];
};

#endif
