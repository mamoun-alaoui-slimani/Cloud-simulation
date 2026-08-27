#pragma once
#include "Sky.h"
#include "PotentialField.h"
#include "MountainChain.h"
#include "Drawable.h"

class System : public Drawable {
private:
    PotentialField field;
    Sky sky;
    MountainChain terrain; // Collection de montagnes

public:
    System(PotentialField const& cp, MountainChain& chain);
    /* Not copyable: MountainChain owns a vector<unique_ptr<Mountain>>,
       which cannot be copied without a virtual clone() on Mountain. */
    System(System const&) = delete;
    System& operator=(System const&) = delete;
    std::ostream& print(std::ostream& out) const;

    /* Physics and rendering are kept apart on purpose. solve() and step()
       touch no renderer, so they can run off the UI thread; drawOn() is
       the only entry point that draws, and it is called from paintGL. */

    /// Solves the flow field and builds the initial sky. Physics only.
    void solve(double threshold = 1e-4, int maxIterations = 5000, bool print = false);

    /// Advances the sky by one step. Physics only.
    void step();

    void drawOn(Renderer& renderer) override;
};

std::ostream& operator<<(std::ostream& out, System const& system);
