# Geometry2D
Geometry tools for intersection and collision detection in 2D.

## What is handled

Includes swept collision detection and resolution for any combination of:
* Axis-aligned rectangles
* Convex polygons
* Circles

With collision types:
* Deflecting collisions that "glance" or "slide" off objects.
* Reversing collisions that reverse direction on collision.
* Reflecting collisions that "bounce" off objects.

The focus is on using swept collisions for accurate results for all types of objects.

## What is not handled

The system does not attempt to implement rotating objects, or any physics.

## Building the library

`make all` will compile the library to `lib/libgeom.a`.
Use `make debug` to get the debugging version.

`make runtest` builds and runs all tests for the project.

Run `make help` for a list of options.

## Using the library

Include `Geometry.hpp`. The library uses a namespace named `geom`.

Link the library with `-LGeometry/lib/ -lgeom`.

If using the library with a multithreaded application, compile with the option `THREADED=TRUE`,
or `#define THREADED` *before* including `Geometry.hpp` in your project.

For some examples of the project, see [CollisionPlayground2D](https://github.com/Claytorpedo/CollisionPlayground2D).
