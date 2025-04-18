# 📘 Flouride Language Documentation

A simple, expressive programming language with functional-style syntax and compact control structures.

## ⚠️ Warning

This language is still in development and is not yet stable so act with caution.
Flouride is not yet recommended for production use.
This documentation is incomplete and will be updated as the language grows.

## Installation

Supported Platforms:

- Ubuntu
- Debian
- Gentoo
- NixOS
- Arch Linux
- Fedora
- OpenSuse
- Solus
- OSX (Very unstable!)
- Manual Install

Debian and Ubuntu:

```sh
sudo apt install flouride
```

Gentoo:

```sh
emerge flouride
```

NixOS:

```sh
nix-env -iA nixpkgs.flouride
```

Arch Linux:

```sh
sudo pacman -S flouride
```

Fedora:

```sh
sudo dnf install flouride
```

OpenSuse:

```sh
sudo zypper install flouride
```

Solus:

```sh
sudo eopkg install flouride
```

Build from Source:

```sh
# First clone the git repo
git clone https://github.com/arcynx/flouride.git
# Then CD into the repo
cd flouride
# Compile (Recomended to use -j flag for faster builds)
make
# Install to the system
sudo make install
```

or as a single command:

```sh
git clone https://github.com/arcynx/flouride.git && cd flouride && make && sudo make install
```

OSX (Use with caution!):

```sh
brew install flouride-unstable
```

## ✨ Basics

### File Extension

```

.fl

```

## 📦 Imports

### Use Standard or External Libraries

```

use std::io
extern test.fl

```

## 🧠 Functions

### Define a Function

```

fn add(a, b)
{
ret -> a + b;
}

```

### Export a Function

Use `@` to export:

```

fn @add(a, b)

```

### Return Value

```

ret -> value;

```

## 🔣 Types & Constants

### Custom Types

```

type bool -> 1 BIT;

```

### Constants

```

def true -> 0x1;
def false -> 0x0;

```

## 🏗️ Variables

### Declare with Type

```

let a = 0.4: int;

```

### Assign From Function Result

```

add -> a, 1 => a;

```

## 🧾 Classes

### Define a Class

```

class vec2
{
public ->

    float x;
    float y;

    fn multipy(value)
    {
        x *= value;
        y *= value;
    }

}

```

## Calling functions

```

println -> "Hello, World!";

```

## 🔁 Loops

### For Loop

```

for -> 0..3, i
{
println -> "Number: ";
}

```

### While Loop

```

while -> a / 2.0 < 10.0
{
add -> a, 1 => a;
}

```

## 📌 Main Function

The main function has a type of int and get's executed at execution it has two optinal parameters let argc and argv to make the function be succesfull 0 needs to be returned.

```

fn main(): int
{
let a = 0: int;
println -> "Start!";
ret -> 0;
}

```

## ❔Conditional Statements

### Format

```

if -> [condition]
{
[do something if condition is met]
}
else
{
[do something if condition is not met]
}

```

### Example

```

if -> x < y
{
println -> "Y is greater than X.";
}
else if -> x > y
{
println -> "X is greater than Y.";
}
else
{
println -> "X and Y are equal.";
}

```

## Match Statements

### Example

```

let value = 1;

match -> value
{
1 => println -> "1";
2 => println -> "2";
3 => println -> "3";
\_ => println -> "Something Else";
}

```

## Selective Imports

```

use std::io -> println
/_ Only println function is imported _/

```

## Import C libaries

```

use SDL2::sdl2.h
/_ Import C function _/

```

## Metadata and annotations

Metadata and annotations like `@[test]`

````fl

@[test]
fn main(): int
...

```fl

## Multitype Arrays

```fl

let array[] = [{int, string}, 3] -> "Hello, World!", 1, 2;

println -> array[1]; /_ Prints Hello World! _/

````

## Creating new variables inside functions

```fl

println -> (vec2) -> 4, 3;

```

## Not worth a whole segment:

Functions can be used inside other functions:

```fl

fn main(): int
{
    fn add(a, b)
    {
        ret -> a + b;
    }
}
```

Classes:

```fl

class vec2
{
    public ->

    float x;
    float y;

    fn add(value)
    {
        /* ... */
    }

    private ->

    float buf;
}

```

## Roadmap

- Finish Documentation
- Add more examples
- Add libaries for strings and math
- Add support for windows and make OSX stable
- Add an option for debugging symbols for gdb

## Also working but not documented yet...

- Generics: `class Box<T>`, `fn identity<T>(val: T): T`
- Operator overloading for classes
- Closures and inline functions
- Built-in testing framework

# Copyright

The Flouride Language is Copyright (c) 2025 Arcynx.

The Flouride Language is distributed under the [MIT License](https://github.com/arcynx/flouride/blob/main/LICENSE).
