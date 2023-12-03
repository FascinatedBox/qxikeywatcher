QXIKeyWatcher
=============

### Overview

QXIKeyWatcher is a library that allows Qt applications to subscribe to key
presses. QXIKeyWatcher uses XInput to listen for keypresses, and then sends a
Qt signal when user-specified keypresses are activated. Key differences between
this and QKeyPress:

* QXIKeyWatcher listens to **all** keypresses that are entered, not just presses
  for a specific window.

* QXIKeyWatcher **does not** grab keys or key combinations. Applications work
  the same with or without QXIKeyWatcher listening.

* QKeyPress is cross-platform, whereas this expects the user to have X11 as the
  display server (usually Linux). I don't know if this works on Wayland, but I
  also have no interest in Wayland.

* This can only track keypresses. It does not track modifiers as well.

Also, it should be noted that QXIKeyWatcher uses a QThread under the hood to
listen for X events. It will not block the main thread.

### Usage

Create a QXIKeyWatcher, then provide textual names of keysyms for it to watch.
Once ready, use start to begin the thread. Because this can be used as a
keylogger, I have not included a demo or examples.

### Credit

Key watching code comes from xkbcat. All I did was wrap that code to allow Qt
to understand it.

### Why

I created this to add simple global hotkeys to future applications of mine.

### Miscellaneous

Keysyms are defined in `X11/keysymdefs.h`, which is probably in `/usr/include/`
but may be somewhere else. Anything in that file can be used, simply remove the
`XK_` prefix.
