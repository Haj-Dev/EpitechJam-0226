/*
** EPITECH PROJECT, 2026
** EpitechJam-0226
** File description:
** AnimationController
*/

#pragma once
#include <cstddef>

class AnimationController {
public:
    AnimationController(size_t frameCount = 1, float frameTime = 0.1f);
    void update(float dt);
    size_t getCurrentFrame() const;
    void reset();
    void setPlaying(bool playing);
    void setFrame(size_t frame);

private:
    size_t _frameCount;
    size_t _currentFrame;
    float _frameTime;
    float _accumulator;
    bool _playing;
};
