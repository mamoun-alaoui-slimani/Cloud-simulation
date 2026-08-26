#pragma once
#include "Mountain.h"


class GaussianPeak: public Mountain{
public:
    GaussianPeak(double i0, double j0, double h, double sx, double sy);
    virtual double altitude(double i, double j) const override;
    virtual std::ostream& print(std::ostream& out) const override;
    virtual void drawOn(Renderer& renderer) override;
};