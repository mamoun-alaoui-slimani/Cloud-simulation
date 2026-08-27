# Orographic Cloud Formation Simulation

[![tests](https://github.com/mamoun-alaoui-slimani/Cloud-simulation/actions/workflows/ci.yml/badge.svg)](https://github.com/mamoun-alaoui-slimani/Cloud-simulation/actions/workflows/ci.yml)

A C++ simulation of cloud formation over mountainous terrain, with two
interchangeable renderers: a text view and a real-time 3D OpenGL view.

Written for the second-year programming project at EPFL (BA2), in C++17.

![The simulation rendered in 3D: a Gaussian peak with grass, rock and snow
textures, and cloud masses condensing over the summit](docs/screenshot.png)

Orbiting the scene with the arrow keys, captured from the running program:

![A full rotation around the mountain, clouds banked against the windward
slope and capping the summit](docs/rotation.gif)

## What it simulates

Wind flowing over a mountain range is forced upward. As the air rises it
expands and cools; when the partial pressure of water vapour exceeds the
saturation pressure, a cloud forms. The simulation solves the flow field
over the terrain, then evolves a 3D grid of air parcels and marks which
ones become cloudy.

Physical parameters (gas constant, molar masses of dry air and water
vapour, ground temperature, humidity, free-stream velocity) live in
`general/constants.h`.

## Architecture

**Terrain, via the Composite pattern.** `Mountain` is an abstract base
with a pure virtual `altitude(i, j)`. `GaussianPeak` is a single Gaussian
peak. `MountainChain` both *is* a `Mountain` and *contains* a
`vector<unique_ptr<Mountain>>`, so a chain of mountains behaves exactly
like a single mountain and can nest arbitrarily. The test suite exercises
that nesting directly.

**Physics on a 3D grid.** `Grid3D<T>` is a class template providing
shared 3D grid storage. `PotentialField : Grid3D<Potential>` solves
the velocity-potential field by iterative relaxation until convergence.
`Sky : Grid3D<AirCell>` holds one air parcel per cell. `System`
owns the terrain, field and sky, and drives the simulation.

**Rendering, via double dispatch (Visitor).** `Drawable` declares
`drawOn(Renderer&)`; `Renderer` declares one `draw(...)`
overload per drawable type. An object does not know how to draw itself,
only how to ask a renderer to draw it. This decouples the physics model
from any renderer, which is why two completely different views,
`TextRenderer` and `OpenGLRenderer`, work against the same simulation with no
changes to the physics code.

## Project structure

    general/   physics and model, built as a static library (libgeneral.a)
    text/      console renderer and entry point
    tests/     unit tests, no external dependency
    Qt_GL/     OpenGL renderer and entry point (GLSL shaders, textures)

## Building

Requires Qt 6 and a C++17 compiler. The OpenGL renderer uses
`QOpenGLTexture` and the `openglwidgets` module, so it does not build
against Qt 5.

    qmake
    make

`Cloud-simulation.pro` is a `subdirs` project: it builds `general/` first
into a static library, then links the front-ends against it. The result is
three executables: `text/cloudsim-text`, `tests/cloudsim-tests` and
`Qt_GL/cloudsim-gl`.

Only the OpenGL front-end needs Qt. The text version builds with a bare
compiler:

```sh
g++ -std=c++17 -I general general/*.cc text/*.cc -o cloudsim-text
```

## Running

**Text version.** One row per grid cell: `i j k`, then squared velocity,
enthalpy, temperature, pressure, vapour pressure, saturation pressure and
the cloud flag.

```
$ ./text/cloudsim-text
1 1 1 405.193 285872 284.491 101313 1286.37 1312.5 0
1 1 2 411.832 285862 284.481 101301 1286.21 1311.65 0
```

**3D version.** `./Qt_GL/cloudsim-gl.app/Contents/MacOS/cloudsim-gl` on
macOS, `./Qt_GL/cloudsim-gl` elsewhere. The relaxation solver runs first,
so the first frame takes a few seconds to appear.

| Key | Action |
|-----|--------|
| Arrow keys | Rotate the camera |
| `W` / `S`, or Page Up / Page Down | Move forward and back |
| `A` / `D` | Move left and right |
| `R` / `F` | Move up and down |
| `Q` / `E` | Roll |
| `Home` | Reset the camera |
| `Space` | Pause and resume the simulation |

## Tests

```sh
./tests/cloudsim-tests
```

No test framework is required, and no Qt either: the suite builds against
the physics library alone.

    g++ -std=c++17 -I general -I text \
        general/*.cc text/TextRenderer.cc tests/main_tests.cc -o cloudsim-tests

What it covers, and why each one is there:

| Area | The check |
|------|-----------|
| Gaussian profile | Summit height, symmetry, and the cutoff that flattens the plain |
| Composite | A chain takes the max of its parts, and a chain nested inside a chain behaves as one mountain |
| Thermodynamics | At ground level and free-stream wind speed, temperature and pressure return **exactly** to their values at infinity |
| Solver, flat terrain | With no obstacle the solved field is exactly the uniform free stream |
| Solver, with a mountain | The flow is lifted and accelerated over the summit, so the test above cannot pass on a solver that does nothing |
| End to end | The shipped configuration yields 21952 interior cells, 361 cloudy, 2002 buried, and is stable across a step |
| Grid | A deliberately non-cubic grid, which a transposed axis order would read out of bounds |
| Bounds | Every out-of-range index reports a null potential instead of indexing off the end |
| Double dispatch | A spy renderer counts the overloads, and `Drawable*` dispatches polymorphically |
| Console renderer | Output goes to an injected stream |

CI builds the console simulation, runs the suite, then runs it again under
AddressSanitizer and UndefinedBehaviorSanitizer. Adding `-fsanitize=address,undefined`
to the command above reproduces that locally. A second job builds every
target with Qt 6, and a third checks that Doxygen runs clean.

## Notes and known limitations

- The surface of a cloud puff is a decorative procedural shape
  (`OpenGLRenderer::cloudSurface`), not simulation output. What is physical
  is *where* the puffs appear: the air parcels that `Sky::isCloudy()` marks
  as condensed.
- Solving the flow field runs on a worker thread, so the window stays
  responsive; the scene is drawn once the solver finishes.


## Licence

MIT. See `LICENSE`.
