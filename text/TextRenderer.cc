#include "TextRenderer.h"
#include "Mountain.h"
#include "Sky.h"

void TextRenderer::draw(Mountain const& mountain) {
    stream << mountain << std::endl;
}

void TextRenderer::draw(Sky const& sky) {
    stream << sky << std::endl;
}
