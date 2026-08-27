#pragma once

class Mountain;
class Sky;

/**
 * @brief Interface of a drawing surface (console, OpenGL window, file...).
 *
 * Second half of the double dispatch: one draw() overload per drawable
 * type. Adding a new view requires no change to the model.
 */
class Renderer {
public:
    virtual void draw(Mountain const& mountain) = 0;
    virtual void draw(Sky const& sky) = 0;

    Renderer() = default;
    virtual ~Renderer() = default;
    // renderers are not copied, but they can be moved
    Renderer(Renderer const&) = delete;
    Renderer& operator=(Renderer const&) = delete;
    Renderer(Renderer&&) = default;
    Renderer& operator=(Renderer&&) = default;
};
