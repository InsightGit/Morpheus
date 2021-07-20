===============
Getting Started
===============

----------------------------------
Installing the DevKitPro toolchain
----------------------------------

Before downloading Morpheus, we need to install the
DevKitPro toolchain that will be used to cross-compile our
code for the Game Boy Advance (GBA) and the DS. In order to install
the necessary components needed, follow
`this guide <https://devkitpro.org/wiki/Getting_Started>`_ and make sure
to install the GBA and the DS (or NDS) developer libraries (also known
as gba-dev and nds-dev).

-------------------
Installing Python 3
-------------------

For many of the build scripts to function, a working Python 3 installation
is needed. While the scripts have only been tested on Python 3.8 and Python
3.9, any Python version above 3.6 should work fine (If you still run
into trouble with 3.6 or 3.7, install one of the tested versions).
You can install Python from `The Python Software Foundation's website
<https://www.python.org/downloads/>`_ or through your preferred package
manager (sudo apt install python3 on Debian/Ubuntu).

----------------
Installing CMake
----------------

For building Morpheus projects, we strongly recommend that you use CMake
as it is the easiest and most cross-platform way to build your Morpheus
projects. You can install CMake from
`cmake.org <https://cmake.org/download/>`_ or through your preferred
package manager (sudo apt install cmake on Debian/Ubuntu).

-------------------
Installing Morpheus
-------------------

Now that the toolchain, CMake, and Python 3 has been installed, we can now
install Morpheus! Go

---------------------------
Creating your first project
---------------------------

