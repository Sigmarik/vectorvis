#include "simulation.h"

#include <string.h>

Molecule::Molecule(const Vec2d& position, const Vec2d& velocity,
                   MoleculeType type)
    : type_(type), position_(position), velocity_(velocity) {}

static const double VOLUME_WIDTH = 1.0;
static const double VOLUME_HEIGHT = 1.0;  // TODO: Replace with variable piston

#define TEST_COLLISIONS 1

static const double GRAVITY = TEST_COLLISIONS ? 20.0 : 0.0;

void Molecule::tick(GasVolume& volume, double delta_time) {
    velocity_ += Vec2d(0.0, GRAVITY * delta_time);
    position_ += velocity_ * delta_time;
}

double Molecule::round() {
    double delta_pres = 0.0;

    int bounces = 0;

    bounces = (int)floor(position_.get_x() / VOLUME_WIDTH);
    double new_x = position_.get_x() - VOLUME_WIDTH * bounces;
    position_.set_x((bounces & 1) ? 1.0 - new_x : new_x);
    if (bounces & 1) velocity_.set_x(-velocity_.get_x());

    delta_pres += mass_ * abs(velocity_.get_x());

    bounces = (int)floor(position_.get_y() / VOLUME_HEIGHT);
    double new_y = position_.get_y() - VOLUME_WIDTH * bounces;
    position_.set_y((bounces & 1) ? 1.0 - new_y : new_y);
    if (bounces & 1) velocity_.set_y(-velocity_.get_y());

    delta_pres += mass_ * abs(velocity_.get_y());

    return delta_pres;
}

static const double MOLECULE_RADIUS = 0.01;

static const double BOUNCINESS = TEST_COLLISIONS ? 0.95 : 1.0;
static const double REACTION_THRESHOLD = TEST_COLLISIONS ? INFINITY : 1.0;

void Molecule::collide(Molecule& molecule, GasVolume& volume) {
    double distance = (position_ - molecule.position_).length();
    if (distance > MOLECULE_RADIUS * 2.0) return;

    Vec2d normal = (position_ - molecule.position_).normalize();

    Vec2d shift = normal * ((MOLECULE_RADIUS * 2.0 - distance) / 2.0);
    position_ += shift;
    molecule.position_ -= shift;

    round();
    molecule.round();

    Vec2d center_velocity =
        (velocity_ * mass_ + molecule.velocity_ * molecule.mass_) /
        (mass_ + molecule.mass_);
    Vec2d this_delta = velocity_ - center_velocity;
    Vec2d that_delta = molecule.velocity_ - center_velocity;

    if (dot(this_delta, normal) > 0.0 || dot(that_delta, normal) < 0.0) {
        return;
    }

    Vec2d this_change = this_delta - normal * (dot(this_delta, normal) * 2.0);
    Vec2d that_change = that_delta - normal * (dot(that_delta, normal) * 2.0);

    velocity_ = center_velocity + this_change * BOUNCINESS;
    molecule.velocity_ = center_velocity + that_change * BOUNCINESS;

    double force =
        this_delta.length() * mass_ + that_delta.length() * molecule.mass_;

    if (force > REACTION_THRESHOLD) {
        volume.react(*this, molecule);
    }
}

static const unsigned COLLISION_CHECKS = 5;

void GasVolume::tick(double delta_time) {
    pressure_ = 0.0;

    if (valve_in_open_) {
        for (unsigned id = 0; id < (unsigned)(delta_time * 1000.0); ++id) {
            add_light_molecule(
                LightMolecule(Vec2d((double)(rand() % 1024) / 1024.0, 0.0),
                              Vec2d::radial(rand(), 1.0)));
        }
    }

    if (valve_out_open_ && molecules_.size() > 0) {
        size_t index = (size_t)rand() % molecules_.size();
        molecules_[index].set_alive(false);
    }

    memset(grid_, 0, sizeof(grid_));

    for (size_t id = 0; id < molecules_.size(); ++id) {
        molecules_[id].tick(*this, delta_time);
        pressure_ += molecules_[id].round();
        pin(molecules_[id]);
    }

    size_t count = molecules_.size();

    for (unsigned check_id = 0; check_id < COLLISION_CHECKS; ++check_id) {
        for (size_t id_a = 0; id_a < count; id_a++) {
            process_collisions(molecules_[id_a]);
        }
    }

    molecules_.filter();

    update_stats();
}

void GasVolume::react(Molecule& alpha, Molecule& beta) {
    if (!alpha.is_alive() || !beta.is_alive()) return;

    Vec2d origin = (alpha.get_position() + beta.get_position()) / 2.0;
    Vec2d avg_vel = (alpha.get_velocity() * alpha.get_mass(),
                     beta.get_velocity() * beta.get_mass()) /
                    (alpha.get_mass() + beta.get_mass());

    if (alpha.get_type() == MT_LIGHT || beta.get_type() == MT_LIGHT) {
        add_heavy_molecule(
            HeavyMolecule(origin, avg_vel, alpha.get_mass() + beta.get_mass()));
    } else {
        for (double spent_mass = 0.0;
             spent_mass < alpha.get_mass() + beta.get_mass();
             spent_mass += 1.0) {
            Vec2d kick = Vec2d::radial(rand(), 1.0);
            add_light_molecule(
                LightMolecule(origin + kick * 0.001, avg_vel + kick));
        }
    }

    alpha.set_alive(false);
    beta.set_alive(false);
}

void GasVolume::update_stats() {
    if (molecules_.size() == 0) return;

    energy_ = 0.0;
    double velocity_sum = 0.0;

    for (size_t id = 0; id < molecules_.size(); ++id) {
        Molecule& molecule = molecules_[id];

        energy_ += molecule.get_mass() * molecule.get_velocity().length2();
        velocity_sum += molecule.get_velocity().length();
    }

    temperature_ = velocity_sum / (double)molecules_.size();
}

void GasVolume::process_collisions(Molecule& molecule) {
    unpin(molecule);

    int x_coord =
        (int)floor(molecule.get_position().get_x() * (double)GRID_RESOLUTION);
    int y_coord =
        (int)floor(molecule.get_position().get_y() * (double)GRID_RESOLUTION);

    for (int x_shift = -1; x_shift <= 1; ++x_shift) {
        if (x_coord + x_shift < 0 ||
            (size_t)(x_coord + x_shift) >= GRID_RESOLUTION) {
            continue;
        }

        for (int y_shift = -1; y_shift <= 1; ++y_shift) {
            if (y_coord + y_shift < 0 ||
                (size_t)(y_coord + y_shift) >= GRID_RESOLUTION) {
                continue;
            }

            Molecule** cell = grid_[x_coord + x_shift][y_coord + y_shift];
            for (unsigned id = 0; id < MAX_CELL_CAPACITY; ++id) {
                if (cell[id] == NULL) continue;
                molecule.collide(*cell[id], *this);
                //* To be honest, here we should unpin each molecule of the
                //* cell and pin it after the collision processing is done,
                //* but... Lets just say that cases in which this correction
                //* would change the simulation in any way are extremely
                //* rare.
            }
        }
    }

    pin(molecule);
}

LightMolecule::LightMolecule() : Molecule(Vec2d(), Vec2d(), MT_LIGHT) {}

LightMolecule::LightMolecule(const Vec2d& position, const Vec2d& velocity)
    : Molecule(position, velocity, MT_LIGHT) {}

void MoleculeStorage::filter() {
    for (size_t id = light_molecules_.size() - 1; id != (size_t)-1; --id) {
        if (id < light_molecules_.size() && !light_molecules_[id].is_alive()) {
            light_molecules_[id] =
                light_molecules_[light_molecules_.size() - 1];
            light_molecules_.pop();
        }
    }

    for (size_t id = heavy_molecules_.size() - 1; id != (size_t)-1; --id) {
        if (id < heavy_molecules_.size() && !heavy_molecules_[id].is_alive()) {
            heavy_molecules_[id] =
                heavy_molecules_[heavy_molecules_.size() - 1];
            heavy_molecules_.pop();
        }
    }

    update_size();
}

HeavyMolecule::HeavyMolecule() : Molecule(Vec2d(), Vec2d(), MT_LIGHT) {}

HeavyMolecule::HeavyMolecule(const Vec2d& position, const Vec2d& velocity,
                             double mass)
    : Molecule(position, velocity, MT_HEAVY) {
    mass_ = mass;
}
