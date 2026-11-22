# PyWright IDE - Experimental C++ Port

This is mainly a curiousity to see what it takes to port PyWright IDE to C++. Everything is in a work-in-progress state.

## What works?

* Opening up games
* Browsing through the directory view
* Text editing and some basic functionality (undo/redo/etc.)

## What doesn't work/isn't implemented?

* Asset Browser in its entirety
* Character Viewer Dialog
* Find/Replace Dialog
* WrightScript Lexer
* Welcome Screen
* IDE Settings

Implemented functionality isn't thouroughly tested either. It is recommended that you shouldn't develop your projects with this version.

## Building

You'll need to have Qt6 installed. Currently the only other dependency, scintilla, is bundled in this repo and will be built statically to the `pywrightide` executable.

Build the project with the usual commands:

    mkdir build && cd build
    cmake ..
    make

Haven't tried building this on Windows though.

## License
Its the same with the Python version of PyWright IDE:

Code is licensed under **GPL v3**. See [LICENSE](LICENSE) for details.

Custom-made icons in `/res/icons` and `/res/iconthemes` that are supplied with this program are licensed with **Creative Commons BY/NC/SA**, with the exception of the ones that utilize official artworks in any way
(the icons that has the Blue Badger), "pwicon.png", which comes from PyWright, and its edited versions "ideicon.ico" and "ideicon.png".

For Scintilla's license, please refer to [thirdparty/scintilla/License.txt](thirdparty/scintilla/License.txt).

## Credits
Please refer to [CREDITS.md](CREDITS.md)!