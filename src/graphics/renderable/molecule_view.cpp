#include "molecule_view.h"

#include <SFML/Graphics/VertexArray.hpp>

#include "sf_cheatsheet.hpp"

MoleculeView::MoleculeView(const GasVolume& volume, Vec2d center, Vec2d size)
    : center_(center), size_(size), volume_(volume) {}

static const double MOLECULE_VIS_SIZE = 0.05;

void MoleculeView::render(MatrixStack<Mat33d>& stack, sf::RenderTarget& target,
                          const AssetShelf& assets) {
    static sf::VertexArray shape(sf::PrimitiveType::Quads, 4);

    shape[0].texCoords = sf::Vector2f(0.0, 0.0);
    shape[1].texCoords = sf::Vector2f(1.0, 0.0);
    shape[2].texCoords = sf::Vector2f(1.0, 1.0);
    shape[3].texCoords = sf::Vector2f(0.0, 1.0);

    for (size_t id = 0; id < volume_.molecule_count(); ++id) {
        Molecule& molecule = volume_.get_molecule(id);

        Vec2d molecule_center =
            center_ - size_ * 0.5 + molecule.get_position() * size_;
        Vec2d molecule_size = Vec2d(MOLECULE_VIS_SIZE, MOLECULE_VIS_SIZE) *
                              sqrt(molecule.get_mass());

        shape[0].position = to_Vector2f(
            stack.top() *
            extrude(molecule_center + molecule_size * Vec2d(-1.0, 1.0) / 2.0));
        shape[1].position = to_Vector2f(
            stack.top() *
            extrude(molecule_center + molecule_size * Vec2d(1.0, 1.0) / 2.0));
        shape[2].position = to_Vector2f(
            stack.top() *
            extrude(molecule_center + molecule_size * Vec2d(1.0, -1.0) / 2.0));
        shape[3].position = to_Vector2f(
            stack.top() *
            extrude(molecule_center + molecule_size * Vec2d(-1.0, -1.0) / 2.0));

        target.draw(shape);
    }
}

ValveControlButton::ValveControlButton(GasVolume& volume, ValveType type,
                                       Vec2d center, Vec2d size,
                                       const char* text)
    : Button(center, size, text), volume_(volume), type_(type) {}

void ValveControlButton::on_push(Interaction interaction) {
    switch (type_) {
        case VALVE_IN:
            volume_.set_in_value(true);
            break;
        case VALVE_OUT:
            volume_.set_out_value(true);
            break;
        default:
            break;
    }
}

void ValveControlButton::on_release(Interaction interaction) {
    switch (type_) {
        case VALVE_IN:
            volume_.set_in_value(false);
            break;
        case VALVE_OUT:
            volume_.set_out_value(false);
            break;
        default:
            break;
    }
}
