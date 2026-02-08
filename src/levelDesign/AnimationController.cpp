/*
** EPITECH PROJECT, 2026
** EpitechJam-0226
** File description:
** AnimationController
*/

#include "AnimationController.hpp"

AnimationController::AnimationController(size_t frameCount, float frameTime)
    : _frameCount(frameCount), _frameTime(frameTime),
      _currentFrame(0), _accumulator(0.0f), _playing(true) {
}

void AnimationController::update(float dt) {
    if (!_playing || _frameCount <= 1) return;

    _accumulator += dt;
    while (_accumulator >= _frameTime) {
        _accumulator -= _frameTime;
        _currentFrame = (_currentFrame + 1) % _frameCount;
    }
}

size_t AnimationController::getCurrentFrame() const {
    return _currentFrame;
}

void AnimationController::reset() {
    _currentFrame = 0;
    _accumulator = 0.0f;
}

void AnimationController::setPlaying(bool playing) {
    _playing = playing;
}

void AnimationController::setFrame(size_t frame) {
    if (_frameCount > 0) {
        _currentFrame = frame % _frameCount;
        _accumulator = 0.0f;
    }
}
