/*
** EPITECH PROJECT, 2026
** EpitechJam-0226
** File description:
** Button
*/

#pragma once

#include <Dualie/Dualie.hpp>

#include <functional>
#include <string>

class Button : public dl::Drawable {
  public:
    Button(const dl::SpriteSheet& spriteSheet, size_t spriteIndex, const dl::Vector2f& size, dl::TextBuffer& textBuffer, const std::string& label);

    void                  setPosition(const dl::Vector2f& position);
    void                  setLabel(const std::string& label);
    void                  update();
    void                  draw(const dl::Vector2f& viewOffset) override;

    std::function<void()> onClick;

  private:
    dl::Sprite    _sprite;
    dl::Text      _label;
    dl::Vector2f  _size;
    dl::FloatRect _bounds;
};
