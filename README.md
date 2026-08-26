# Orographic Cloud Formation Simulation

A C++ simulation of cloud formation over mountainous terrain, with two
interchangeable renderers: a text view and a real-time 3D OpenGL view.

Written for the second-year programming project at EPFL (BA2), in C++17.

![The simulation rendered in 3D: a Gaussian peak with grass, rock and snow
textures, and cloud masses condensing over the summit](docs/screenshot.png)

## What it simulates

Wind flowing over a mountain range is forced upward. As the air rises it
expands and cools; when the partial pressure of water vapour exceeds the
saturation pressure, a cloud forms. The simulation solves the flow field
over the terrain, then evolves a 3D grid of air parcels and marks which
ones become cloudy.

Physical parameters (gas constant, molar masses of dry air and water
vapour, ground temperature, humidity, free-stream velocity) live in
`general/constantes.h`.

## Architecture

**Terrain, via the Composite pattern.** `Montagne` is an abstract base
with a pure virtual `altitude(i, j)`. `Gaussienne` is a single Gaussian
peak. `ChaineDeMontagnes` both *is* a `Montagne` and *contains* a
`vector<unique_ptr<Montagne>>`, so a chain of mountains behaves exactly
like a single mountain and can nest arbitrarily. The test suite exercises
that nesting directly.

**Physics on a 3D grid.** `Collection3D<T>` is a class template providing
shared 3D grid storage. `ChampPotentiels : Collection3D<Potentiel>` solves
the velocity-potential field by iterative relaxation until convergence.
`Ciel : Collection3D<CubedAir>` holds one air parcel per cell. `Systeme`
owns the terrain, field and sky, and drives the simulation.

**Rendering, via double dispatch (Visitor).** `Dessinable` declares
`dessine_sur(SupportADessin&)`; `SupportADessin` declares one `dessine(...)`
overload per drawable type. An object does not know how to draw itself,
only how to ask a renderer to draw it. This decouples the physics model
from any renderer, which is why two completely different views,
`TextViewer` and `VueOpenGL`, work against the same simulation with no
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

No test framework is required. The suite covers the Gaussian terrain
profile, the Composite nesting of mountain chains, the thermodynamics of
an air parcel against its analytic limiting case (at ground level and at
the free-stream wind speed, temperature and pressure must come back
exactly to their values at infinity), grid dimensions, the double-dispatch
rendering path, and the console renderer. It exits non-zero on failure, so
it drops straight into CI.

## Notes and known limitations

- The OpenGL renderer uses the legacy fixed-function pipeline
  (`glBegin`/`GL_QUADS`, GLSL 1.20). It works, including on Apple's
  Metal-backed OpenGL 2.1, but a modern rewrite around vertex buffers
  would be the natural next step.
- `Collection3D` stores its grid as `[Nz][Ny][Nx]` while `Ciel` indexes it
  as `[i][j][k]`. The two agree only because the simulation uses a cubic
  grid; a non-cubic grid would need this reconciled first.
- The surface of a cloud puff is a decorative procedural shape
  (`VueOpenGL::formeNuage`), not simulation output. What is physical is
  *where* the puffs appear: the air parcels that `Ciel::Nuageux()` marks
  as condensed.
