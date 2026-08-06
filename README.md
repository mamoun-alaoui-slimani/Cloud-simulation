# Orographic Cloud Formation Simulation

A C++ simulation of cloud formation over mountainous terrain, with two
interchangeable renderers: a text view and a real-time 3D OpenGL view.

Written for the second-year programming project at EPFL (BA2), in C++17.

## What it simulates

Wind flowing over a mountain range is forced upward. As the air rises it
expands and cools; when the partial pressure of water vapour exceeds the
saturation pressure, a cloud forms. The simulation solves the flow field
over the terrain, then evolves a 3D grid of air parcels and marks which
ones become cloudy.

Physical parameters (gas constant, molar masses of dry air and water
vapour, ground temperature, humidity, free-stream velocity) are in
`general/constantes.h`.

## Architecture

**Terrain — Composite pattern.** `Montagne` is an abstract base with a
pure virtual `altitude(i, j)`. `Gaussienne` is a single Gaussian peak;
`ChaineDeMontagnes` both *is* a `Montagne` and *contains* a
`vector<unique_ptr<Montagne>>`, so a chain of mountains behaves exactly
like a single mountain and can nest arbitrarily.

**Physics on a 3D grid.** `Collection3D<T>` is a class template providing
shared 3D grid storage. `ChampPotentiels : Collection3D<Potentiel>` solves
the velocity-potential field by iterative relaxation until convergence;
`Ciel : Collection3D<CubedAir>` holds one air parcel per cell.
`Systeme` owns the terrain, field and sky and drives the simulation.

**Rendering — double dispatch (Visitor).** `Dessinable` declares
`dessine_sur(SupportADessin&)`; `SupportADessin` declares overloads of
`dessine(...)` for each drawable type. An object doesn't know how to draw
itself, only how to ask a renderer to draw it. This decouples the physics
model from any renderer, which is why two completely different views —
`TextViewer` and `VueOpenGL` — work against the same simulation with no
changes to the physics code.

## Project structure

    general/   physics + model, built as a static library (libgeneral.a)
    text/      console renderer + entry point
    Qt_GL/     OpenGL renderer + entry point (GLSL shaders, VBO/IBO, textures)

## Building

Requires Qt 5 or 6 and a C++17 compiler.

    qmake
    make

This produces two executables: `text/ex_05_text` and `Qt_GL/ex_05_gl`.
