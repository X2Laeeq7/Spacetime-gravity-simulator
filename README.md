# 🌌 Gravity Sheet — 3D Spacetime Visualization

An interactive 3D gravity simulator built in **C++** with **raylib**, visualizing how massive objects can curve spacetime.

## ✨ Features

### 🌍 Solar System Mode

* Sun and planets from Mercury to Saturn
* Relative orbital speeds
* Earth–Moon hierarchical orbit
* Asteroid belt
* 3D orbital camera

### 🕳️ Black Hole Mode

* Supermassive black hole
* Event horizon
* Spinning accretion disk
* Photon ring and additional disk layers
* Schwarzschild-inspired spacetime deformation

### 🎨 Visuals

* 1000+ stars with multiple colors and twinkling effects
* 3D spacetime grid
* Depth-based grid brightness
* Smooth camera controls

## 🎮 Controls

| Input           | Action       |
| --------------- | ------------ |
| Left mouse drag | Orbit camera |
| Mouse scroll    | Zoom         |
| ESC             | Exit         |

## 🛠️ Built With

* **C++**
* **raylib**
* **C++17**
* **Git**

## 🚀 Running the Project

### Prerequisites

* Windows 10/11
* MinGW / g++
* raylib 5.0+
* raylib installed at `C:\raylib`

### Compile

Run:

```bash
.\compile.bat
```

### Run

```bash
gravity_sheet.exe solar
```

or

```bash
gravity_sheet.exe blackhole
```

Running the executable without an argument defaults to Solar System mode.

## 🔬 How It Works

The simulation uses a combination of simplified gravitational models and parametric orbital motion.

Massive objects contribute to the deformation of the spacetime grid, while planets and moons follow hierarchical orbital paths.

The black hole mode uses a simplified inverse-square deformation to create the visual effect of a deep gravitational well.

> This is a visualization and is **not a physically accurate General Relativity simulation**.

## 📁 Project Structure

```text
gravity-sheet/
├── src/
│   ├── main.cpp
│   └── CelestialBody.h
├── compile.bat
└── README.md
```

## 🎓 What I Learned

This project was one of my main ways of learning **C++** outside of university.

Through it, I worked with:

* C++ classes and object-oriented programming
* Vectors and references
* 3D graphics and transformations
* Orbital systems and hierarchical objects
* Gravitational simulations
* Camera controls and rendering
* Git and GitHub
* raylib

## 🚧 Limitations

This project is primarily a **visual simulation** rather than a scientifically accurate physics engine.

Some planned improvements include:

* Gravitational lensing using GLSL shaders
* Orbital trails
* Multiple black holes
* Cross-platform builds
* More physically accurate spacetime models

## 📄 License

MIT

## 🙏 Acknowledgments

* [raylib](https://www.raylib.com/) by Ramon Santamaria
* *Interstellar* (2014) for visual inspiration
* Albert Einstein's work on General Relativity
