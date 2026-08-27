#pragma once

#include <iostream>
#include "Renderer.h"

/**
 * @brief Console renderer.
 *
 * The output stream is injected, so output can go somewhere other than
 * std::cout: a file, or a std::ostringstream in the tests.
 */
class TextRenderer : public Renderer {
public:
    explicit TextRenderer(std::ostream& stream = std::cout) : stream(stream) {}

    void draw(Mountain const& mountain) override;
    void draw(Sky const& sky) override;

private:
    std::ostream& stream;
};
