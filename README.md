# slock

My personal build of slock, suckless's screen locker, version 1.4 (see `VERSION` in `config.mk`).

## Patches

There's no `patches/` directory here, but one patch is merged straight into `slock.c`: background image support (the suckless "background image" patch, built on Imlib2 and `libXrandr`). It loads an image with `imlib_load_image`, then for every monitor `XRRGetMonitors` reports, blends and scales that image onto a pixmap sized to that monitor (`slock.c:377-397`). Each monitor gets the same image stretched to fill it, there's no aspect-ratio preservation or cropping.

## Changes beyond stock

- `background_image` in `config.def.h` points at an absolute path, `/home/martin/dev/suckless-btw/slock/slock.png`. Anyone else building this needs to change that to wherever they actually keep the image, or to `slock.png` in this repo if the checkout lives in the same spot.
- `colorname[]` keeps the stock three-state palette (blue while typing, red on a wrong password, black at rest), but see below, that feedback doesn't actually show once a background image is set.

## Known issues

- The comment above `FAILED` in `config.def.h` says "Does not curently work with BG IMG", and that's accurate: `readpw()` in `slock.c` (around line 194) only swaps `XSetWindowBackground` to the plain color when `locks[screen]->bgmap` is unset. Whenever a background image is loaded, every color transition (`XSetWindowBackgroundPixmap(dpy, ..., bgmap)`) redraws the same static image instead, so typing or getting the password wrong never changes what's on screen.
- `make dist` copies a file named `README` with no extension; this repo only has `README.md`, so that target fails at the `cp -R` step.

## Building

Needs Xlib, Xext, Xrandr, Imlib2, and `libcrypt` (drop `-lcrypt` from `config.mk` on OpenBSD or Darwin, and drop `-DHAVE_SHADOW_H` on any non-Linux BSD, both are called out in commented-out lines in `config.mk`).

```sh
make
```

Prints the build options (compiler, `CFLAGS`, `LDFLAGS`), copies `config.def.h` to `config.h` on the first run, and compiles `slock.c` plus `explicit_bzero.c` into an `slock` binary. Edit `config.h`, not `config.def.h`, once it exists, and make sure to point `background_image` at a real path on your machine before building.

Install system-wide:

```sh
sudo make install
```

This needs root because it sets the setuid bit on the installed binary (`chmod u+s`), which slock needs to read `/etc/shadow` for the password hash before it drops privileges. It copies the binary to `${PREFIX}/bin` (`/usr/local/bin` by default) and the man page to `${MANPREFIX}/share/man/man1`. Override `PREFIX`/`DESTDIR` as usual, e.g. `make install PREFIX=/usr`.

`make clean` removes the binary and object files. `make uninstall` removes what `install` put down. `make dist` builds a source tarball (currently broken, see above).

The man page's security section is worth reading before relying on this: to stop the lock being bypassed by switching virtual terminals or killing the X server with Ctrl+Alt+Backspace, add to `xorg.conf`:

```
Section "ServerFlags"
    Option "DontVTSwitch" "True"
    Option "DontZap"      "True"
EndSection
```

## Credits

slock is written and maintained by suckless.org; see `LICENSE` for the full contributor list. The background-image patch traces back to the one published on [suckless.org/patches](https://tools.suckless.org/slock/patches/).
