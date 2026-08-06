#include <iostream> // pour endl
#include "TextViewer.h"
#include "Montagne.h"
#include "Ciel.h"

void TextViewer::dessine(Montagne const& montagne){
   std::cout << montagne << std::endl;
}

void TextViewer::dessine(Ciel const& ciel){ //besoin de la méthode d'affichage sans la montagne en paramètre
    std::cout << ciel << std::endl;
}
