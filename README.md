UPReader
========

A google reader client for Ubuntu Phone

Still in the early stages of development.

It may or may not work for you

To install and run on device
============================

* Open QtCreator, go to MenuBar->Tools->Ubuntu->Device->Enable->Cpp compiling.
Ensure that completes successfully.
* You will need to install cmake on your device. Open a ssh connection to your device (Ctrl-F10 in QtCreator)
and run ````sudo apt-get install cmake````
* Then open the project in QtCreator, go to MenuBar->Build->Ubuntu->Build C++ project on Ubuntu Device. Wait for that to finish successfully.
* Open a ssh connection to your device. You should see a file named up-reader_0.1-1_armhf.deb or similar. Install that using dpkg. eg. ````sudo dpkg -i up-reader_0.1-1_armhf.deb````
* Run ````/usr/bin/UPReader --desktop_file_hint=/usr/share/applications/up-reader.desktop```` on the device

License
=======

A Google Reader client for Ubuntu Phone
Copyright (C) 2013  Jimi Smith <smithj002@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
