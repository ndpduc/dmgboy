**DMGBoy 2.0** (2014-05-11)
  * Added GameBoy Color compatibility
  * Added a 3D model of the original GameBoy inside the window
  * Added a GUI debugger with disassembler, memory viewer and breakpoints
  * Translated the GUI to other languages: Spanish, French and Greek at the moment
  * The window now is resizable in real time, also mantains the proportions and the borders are magnetic
  * Updated wxWidgets to version 3
  * Fixed instruction DAA
  * Improved the LCDC states
  * Improved the timing of the instructions

**DMGBoy 1.0** (2012-10-19)
  * Added a OpenGL renderer. This decrease the use of the cpu when the size of the window is bigger than 1.
  * Added fullscreen mode (only in the OpenGL mode)
  * Improved the audio in Linux
  * Improved the stability in computers with poor performance
  * Improved the internal timer
  * Improved interruptions
  * Other minor improvements in accuracy
  * Changed the default keys to Up, Down, Left, Right, A, S, Shift, Return. This change is because most of the keyboards can't notify the press of 4 keys in the previous configuration. And The Legend of Zelda requires that A, B, Select, Start keys are pressed to save the game
  * Fixed bug in the MBCs
  * Fixed bug in the SBC instruction (The sprite of the main character in the game Battletoads is rendered properly)
  * Fixed the E8, F8 instructions
  * Visual C++ 2010 redistributable included in the windows installer (no need to download it)

**gbpablog 0.9** (2011-06-18)
  * Added sound (`*`)
  * Added support for savestates
  * Improved the way the frames are drawn
  * Fixed bug in MBC
  * Implemented serial port interruption (needed by some games)
  * The SDL library is no more needed to be installed in MacOSX, is included in the app bundle
  * Improved accuracy
  * Improved compatibility

`*` Note:
The sound in ubuntu with pulseaudio is not good enough:
  * Choppy sound with the package libsdl1.2debian-pulseaudio.
  * Delayed sound with the package libsdl1.2debian-alsa but with some games can be acceptable.
  * The emulator hangs with the package libsdl1.2debian-esd.

**gbpablog 0.8** (2011-02-26)
  * Added the possibility to change the keys on the keyboard
  * Added support for drag & drop roms
  * Added load of roms from command line
  * Added a menu list of recent roms
  * Added support for the roms with battery to save the state to a file and restore it in a later execution
  * Fixed bug with interruptions that prevented to start some games or produced graphic errors in others
  * Other minor changes

**gbpablog 0.7** (2010-12-07)
  * Fixed bug in windows intaller.
  * Added preferences dialog:
    * Window size
    * Greenscale / grayscale
  * Added support for zipped roms
  * Added about dialog with number version and url to the webpage

**gbpablog 0.6**: (2010-08-21)
  * First public version released