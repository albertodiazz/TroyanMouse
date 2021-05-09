# TroyanMouse
TroyanMouse

For this project i get control of the mouse from another computers inside the red from a master computer. The master computer control all the mouse events from all the computers inside the local area.

I use C++ for the master app with hook event on windows, socket io and python for the slave with the library pywin32. All works on second planes in async ways. You can compile the code of python with pyInstaler. The C++ was made on visual sytudio 2017.

A big influence for resolve this project:

- https://warroom.rsmus.com/dll-injection-part-1-setwindowshookex/
