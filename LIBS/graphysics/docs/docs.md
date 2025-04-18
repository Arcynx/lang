# Graphysics Documentation

A library for creating interactive simulations in up to 4 Dimensions using the Flouride language.

## ⚠️ Warning

This library is still in development and is not yet stable.
The API may change in the future so this library is not recommended for production use.

This Documentation is incomplete and will be updated as the library grows.

## Installation

First add the library to your project.fld file:

```fld
<LIBARIES>
    [Graphysics]
        (Source)
            "https://github.com/arcynx/graphysics.git"
        (Version)
            "0.0.3"
        (Externals)
            "debug": true # Add this to run in debug mode
```

Then add the library to your main.fl file:

```fl
use scientific::graphysics
```

If you encounter the error: `Error: Library not found: scientific::graphysics` then you need to compile the library before you can use it:

```sh
flcc --LIB -s https://github.com/arcynx/graphysics.git
```

## Basic Usage

### Initialize

Create a window and initialize the library.

```fl
use scientific::graphysics
/* By deault graphysics is stored under the scientific catagory */

fn main()
{
    /*                                  width, height,  title       */
    let window[] = [{int, string}, 3] -> 400, 400, "Example Window"; /* Settings for the window */

    graphysics::init -> window;

    ret -> 0;
}
```

#### Run

```sh
# First compile:

flcc -o out main.fl

# Then run:

./out

```

### Add Objects

```fl
/* Create a node */
let node = (vec3) -> 0, 0, 0;

/* Add the node to the simulation */
graphysics::add -> node;
```

### Run the simulation

```fl
/* Create time */
let time = 0;

/* Run the simulation */
while -> time < 200
{
    advance -> time, 200ms; /* One tick is 200 ms */

    graphysics::render -> (); /* Render the simulation */

    time += 1;
}
```

### Settings

```fl
let settings = (graphysics::settings) -> 3D; /* Create settings for a 3D simulation */

/* Now initialize the simulation with settings */
graphyics::init -> window, settings;
```

### Simulation Settings

```fl
let setts = (graphysics::simulation.settings) -> ();

setts.F = (vec4) -> 1, 1, 1, 1;
```
