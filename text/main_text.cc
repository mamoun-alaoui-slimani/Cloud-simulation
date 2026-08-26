#include <iostream>
#include "TextRenderer.h"
#include "System.h"
#include "GaussianPeak.h"
using namespace std;

int main()
{
  TextRenderer renderer;

  PotentialField potentialField(30, 30, 30, 20.0/29.0);
  std::vector<std::unique_ptr<Mountain>> chain;
  chain.emplace_back(std::unique_ptr<Mountain>(new GaussianPeak(15.0, 15.0, 15.0, 5.0, 5.0)));
  MountainChain terrain(chain);


  System system(potentialField, terrain);
  system.start(renderer);

  return 0;
}
