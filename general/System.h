#pragma once
#include "Sky.h"
#include "PotentialField.h"
#include "MountainChain.h"
#include "Drawable.h"

class System : Drawable{
private:
    PotentialField field;
    Sky sky;
    MountainChain terrain; //Collection de montagnes

public:
    System(PotentialField const& cp, MountainChain& chain);
    /* Not copyable: MountainChain owns a vector<unique_ptr<Mountain>>,
       which cannot be copied without a virtual clone() on Mountain. */
    System(System const&) = delete;
    System& operator=(System const&) = delete;
    std::ostream& print(std::ostream& out) const;
    void update(Renderer& renderer);
    virtual void drawOn(Renderer& renderer) override;
    void start(Renderer& renderer, double threshold = 1e-4, int maxIterations = 5000, bool print = false);
};

std::ostream& operator<<(std::ostream& out, System const& system);
