# ImGuizmo Debug Environment

Basically, a bit of stuff to debug the matrix operations found in [ImGuizmo].

## Requirements

A Lua install including headers compatible with PUC/Rio Lua 5.3.5.

## Usage

Run `make gizmo_matrix.so` to compile the C library.

Then, fiddle with `init.lua` as required.


# Branches

* `spread` requires a few environment files set up that I've yet to release.
* `standalone` has the relevant parts inline.

Unless you're me, you want to use `standalone`.


[ImGuizmo]: https://github.com/CedricGuillemet/ImGuizmo
