#include "TextViewer.h"
#include "Montagne.h"
#include "Ciel.h"

void TextViewer::dessine(Montagne const& montagne){
   flot << montagne << std::endl;
}

void TextViewer::dessine(Ciel const& ciel){
    flot << ciel << std::endl;
}
