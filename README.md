ChipLib
==================
This library was originally written because no suitable FOSS library for reading VLSI chips was readily available -- all the readers were tied into a VLSI library.  
It can only read CIF files (Caltech Intermediate Format), is somewhat messy, and requires Qt (but it shouldn't be too hard to remove that dependency.) It's under the MIT license.

I don't plan on maintaining this, it's here because I needed one and had to spend a significant amount of effort to write it, so I figured that in the future, someone else might need one.

Architecture
---------------------
(Note that this information could be somewhat messy/out-of-date as I'm writing this from memory after not having touched the code for ~6 months or so.)

### `ChipLib`
The library itself is contained in the `ChipLib` directory. There's also a `CMakeLists.txt` file in case you want to use it as a subproject (like [`cpd-CuPixDRC`](https://github.com/waddlesplash/cpd-CuPixDRC) does).

The actual parser code is in [`ChipLib/IO/Cif.cpp`](https://github.com/waddlesplash/cpd-ChipLib/blob/master/ChipLib/IO/Cif.cpp). (CIF as a format is a bit of a mess and isn't like most other vector graphics formats, and I had to do some deciphering and comparing against existing readers in order to figure out how to output it. Using various testcases, I think I managed to get ChipLib rendering nearly pixel-perfect representations of most CIF files).

The storage code is in [`ChipLib/Chip.h`](https://github.com/waddlesplash/cpd-ChipLib/blob/master/ChipLib/Chip.h) and its accompanying `.cpp` file. It basically stores rectangles, polygons, and points on a per-layer basis; which is all CIF really needed. The `Chip` class also takes care of running the renderer and determining the size of any given layer.

The renderer consists of an abstract base class, [`LayerGraphics`](https://github.com/waddlesplash/cpd-ChipLib/blob/master/ChipLib/LayerGraphics.h) (sometimes abbreviated in commit messages and in the source code as `LG`), that is implemented in various ways -- `ChipDisplay` has implementations for `QPainter`, `QGraphicsView`, and `libgd`. Note that you can't just implement a `LayerGraphics` by calling the equivalent functions in any painting library, as each `LayerGraphics` has its own transformation functions to convert from `Chip` coordinates (which are NOT `CIF` coordinates -- that would be madness, have a look at what CIF coordinates are to see why) to the coordinates of whatever painint library it uses. This is also a mess, as a lot of that logic could probably be abstracted out to a renderer-agnostic base class. But I really didn't feel like spending any time on something that worked as I was on a deadline (more about why I did this in the first place in the [other repository](https://github.com/waddlesplash/cpd-CuPixDRC)).

### `ChipDisplay`
ChipDisplay is the GUI app that is used for viewing chips and exporting them into the text-based prerendered format that the design rule checker uses. It supports layer filtering, scaling, SVG exporting, and a few other things that I wound up needing to do at some point or another. It uses a `QGraphicsView` for displaying stuff.

If you have any questions about this whole thing or want to know something more, feel free to open an issue in this repository.

### `CIF` & why reading it is such a pain
Most vector graphics formats just are a list of objects (or a hiearchy of them) that specify coordinates, colors, etc. `CIF` is nearly a language: you can define a "subroutine" of objects, then call it with transformation parameters. This is both ingenious and nasty. Ingenious, because it's an easy way to make files smaller and easier to modify (one MIPS chip that I was testing with was a 4MB CIF file -- uncompressed!) and nasty because you have to run this through an interpreter at load-time before you can render it. For the aforementioned 4MB MIPS chip CIF, `ChipLib`'s interpreter takes about 10 seconds and the resulting memory structure is around ~200MB. (I can't imagine how editing software does this -- the trial copies and free software that I used, the ones that rendered the full CIFs anyway, did this so fast that I can't imagine how they do it. Maybe they have a JIT?!)
