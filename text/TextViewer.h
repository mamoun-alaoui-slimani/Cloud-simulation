#pragma once

//#include <iostream>
#include "SupportADessin.h"

class TextViewer : public SupportADessin {
public:
    /**
 TextViewer(std::ostream& flot)
   : flot(flot)
    {}
**/

  virtual void dessine(Montagne const& montagne) override;
  virtual void dessine(Ciel const& ciel) override;

  /**
private:
  std::ostream& flot;
  **/
};
