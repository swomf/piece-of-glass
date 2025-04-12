# piece-of-glass

I have a very specific want:

* A window that is practically invisible
* An interactive image is rendered on it, that I can smoothly rotate via touchpad gesture

For example, what if I wanted to fidget with
a `cat.png` overlayed right on top of my screen?

This repo is my attempt at a C GTK+ implementation thereof.

## how do i run it?

* Unix-like machine with...
  * **runtime**: gtk3, cairo
  * **build-time**: meson (install via pip or package manager)

Though piece-of-glass is in theory only dependent on GTK, I have only tested it on
Hyprland.  
I'll wait until this has _more_ than one (1) user to consider deeper interoperability testing :]

```shell
meson setup build
ninja -C build
ninja install -C build
```
