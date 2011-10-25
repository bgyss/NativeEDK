Introduction
------------

This is the README.TXT file for the Fabric native extension development
kit (NativeEDK).  This file and all the files in this distribution are 
copyright 2010-2011 Fabric Technologies Inc.

License
-------

The Fabric native extension development kit is open-source and is licensed 
under the New BSD License.  The exact license is contained in the file 
LICENSE.TXT included with this distribution.

Any source files located under the Native/ThirdParty directory are 
open-source third-party libraries; their licenses can be found under
that directory.  Any person or organization that distributes Fabric
extensions build with the NativeEDK must comply with both the NativeEDK
license and the licenses of all the source files under Native/ThirdParty.
It is our intention to include only libraries that use BSD or BSD-like
licenses, but it is the responsibility of the extension developer to
ensure their own compliance with these licenses.  Note that as of this
writing there is only a single third-party library included with the
NativeEDK, namely the YAJL JSON parsing and encoding library.

Getting Started
---------------

The Fabric native extension development kit allows Fabric developers to 
create Fabric extensions in C++ that provide additional functionality
to Fabric applications.  This functionality can include access to local 
resources, exposure of a third-party library to KL operators, or pretty 
much anything else that can be done with native C++ code.

The NativeEDK itself is primarily a single C++ header file that must be
included and used by the extension, along with a build system (using
the Python-based SCons build tool) and a few support libraries 
required by the extension.

To build the sample extension, you need to first set up a development
toolchain on your system.  The requirements are:

- A C++ compiler
- The SCons build tool
- gmake (optional)

The toolchain install instructions are:

- Linux:
  - run: sudo apt-get install g++ scons gmake
- OS X
  - Install MacPorts: http://www.macports.org/install.php
    - Note that this requires installing Xcode
  - run: sudo port install scons
- Windows:
  - TBD

Once the toolchain is installed, you should simply be able to run
'make' in the top-level directory of the NativeSDK and the sample
extension will be built.  By default, the extension will be build
as a debug build; to build it as a release build run
'scons TYPE=Release'.

The extension itself will be located in the directory
'Native/dist/Darwin/universal/Debug/FabricEngine/Library/Fabric/Exts'.
Copy all the files from that directory to the following directory:

- Linux: ~/.fabric/Exts
- OS X: ~/Library/Fabric/Exts
- Windows: TBD

If you then restart your browser the sample extension will be 
available to your Fabric applications.  The lowest-level way of
verifying that the extension has been loaded is to run
`FABRIC.EX.getLoadedExts()` from the Javascript console of your
Fabric application.
