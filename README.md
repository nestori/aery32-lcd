README..ish

code is for aery32, using ssd1289 driven 3" 320x240 resolution lcd, with 40 datapins and resistive touchscreen
code is by Nestori

computer side of code depends on sfml2.0

to compile for aery32:

put everything on your aery32-0.5.0 folder
change #define FORPC to #define FORAERY in lcd_defines.h file


to compile on osx:

g++ -Wall main.cpp lcd_init.cpp lowlvl.cpp lcd_math.cpp lcd_graphics.cpp simscreen.cpp -I/Library/Frameworks/SFML.framework/Headers -framework OpenGL -framework SFML -framework sfml-graphics -framework sfml-system -framework sfml-window -o foraery


to compile on linux etc:

about the same as osx, change -framework stuff to -l
and correct paths to find sfml/opengl etc

to compile on pc, on code::blocks:

install sfml2.0 to your c::b
???
profit!

sorry .__.