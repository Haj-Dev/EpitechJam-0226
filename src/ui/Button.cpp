/*
** EPITECH PROJECT, 2026
** EpitechJam-0226
** File description:
** Button
*/

#include "Button.hpp"

Button::Button(const dl::SpriteSheet& spriteSheet, size_t spriteIndex, const dl::Vector2f& size, dl::TextBuffer& textBuffer, const std::string& label) :
    _label(textBuffer), _size(size), _bounds(0.0f, 0.0f, size.x, size.y) {
    _sprite.loadFromSpriteSheet(spriteSheet, spriteIndex);
    _label.setString(label);
    _label.setAlignment(dl::AlignCenter);
    _label.setColor(dl::Color(20, 20, 20));
}

void Button::setPosition(const dl::Vector2f& position) {
    _sprite.setPosition(position);
    _bounds.left = position.x;
    _bounds.top  = position.y;
    _label.setPosition(position.x + _size.x * 0.5f, position.y + _size.y * 0.35f);
}

void Button::setLabel(const std::string& label) {
    _label.setString(label);
}

void Button::update() {
    if (!dl::Input::isKeyPressed(dl::Input::TOUCH)) {
        return;
    }
    const auto         touch = dl::Input::TouchPosition;
    const dl::Vector2f touchPoint(static_cast<float>(touch.x), static_cast<float>(touch.y));
    if (_bounds.contains(touchPoint) && onClick) {
        onClick();
    }
}

void Button::draw(const dl::Vector2f& viewOffset) {
    _sprite.draw(viewOffset);
    _label.draw(viewOffset);
}
