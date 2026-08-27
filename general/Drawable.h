#pragma once

class Renderer;

/**
 * @brief Interface for objects that can be drawn on a Renderer.
 *
 * First half of the double dispatch: an object does not know how to draw
 * itself, it only knows how to ask a renderer to draw it.
 */
class Drawable {
public:
    virtual void drawOn(Renderer& renderer) = 0;

    Drawable() = default;
    virtual ~Drawable() = default;
    Drawable(Drawable const&) = default;
    Drawable& operator=(Drawable const&) = default;
    Drawable(Drawable&&) = default;
    Drawable& operator=(Drawable&&) = default;
};
