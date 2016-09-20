Free SQL Formatter
====

This is a tool that adds linebreaks and indentation to SQL source code.

Licensed under LGPL.  Full license is available in file 'LICENSE', which you should have received with the program


How it works
----
fsqlf simply goes through provided input text, and tries to recognise a predefined set of keywords.  If a keyword is recognized it adds spacing around it using settings in the configuration file (and to some level settings from CLI/GUI).


Table of content:
----

1.  Current state of development
2.  Compilation
    1.  Requirements
    2.  Compilation process
3.  TODO lists
4.  Usage
    1.  Usage commandline
    2. Usage graphical user interface
5.  Project site
- APPENDIX A - wxWidgets installation



1.  Current state of development
----
My use case is formatting generated SQL code - make something readable out of one long line of SQL code

Current list of implemented capabilities:

-  indent subqueries
-  new line in cases where it seems appropriate (items in SELECT list. Keywords FROM, JOIN, ON, WHERE, AND, OR, DELETE, INSERT, UPDATE, SET)
-  capitalisation of recognised keywords (not all - I like some keywords lowercase, but this should be configurable eventually)
-  gui
-  configurations read from file

(see TODO list what is planned for future)



2.1  Compilation - Requirements
----

The following list actually should be sufficient. Look into 'makefile' for compilation instructions

- flex         # http://flex.sourceforge.net/
- C compiler   #
- wxWidgets    # needed for Graphical User Interface (GUI)
- C++ compiler # for compiling GUI


Stuff that I got installed:

- flex      # http://flex.sourceforge.net/
- gcc
- wxWidgets # needed for Graphical User Interface (GUI). If you want to build Windows executables on Linux machine see 'APPENDIX.1 - wxWidgets installation'
- g++       # for compiling GUI
- make      # This makes all compiling automated
- mingw32   # cross compiler which is used to produce Windows executables under Linux
- zip       # used only for creating zip archive with source code and executable files
- git       # used for version control and creating zip archive with source code and executable files


2.2 Compilation process
----
General instructions

- Use 'flex' on core/fsqlf.lex
- Compile its output with C compiler and name the executable 'fsqlf.exe' if you on Windows and 'fsqlf' in other cases (the name is needed for gui to be able to call it)
- Compile 'gui/wx_fsqlf.cpp' with C++ compiler
- Put both executables 'fsqlf' and 'wx_fsqlf' into one directory.

If you use Linux and have installed the same programs as I have (see section 2.1), then you can use the make file

    # make all      # Compile source for LINUX and WINDOWS. Excutables can be found in 'bin/' directory:  'fsqlf', 'fsqlf.exe', 'gui_wx_basic', 'gui_wx_basic.exe'
    # make test     # Compile and run program on test file 'test.sql' and print output to console
    # make zip      # Create zip archive with executables and source code binaries (for publishing)
    # make clean    # Remove all files created with other make commands


3. TODO list:
----

- parenthesis in restrictions (e.g. AND (a=1 OR b=1) )
- improve GUI
- make 3 configuration modes:
    1. none-use-file-config
    2. detailed-as-in-file
    3. select-templates-of-configurations
- improve analytical function formatting
- alignment of comments
- in equality condition align all comparison signs ('=' , '<>', etc.)


4.1 Usage from command line
----
Windows

    # bin/fsqlf.exe                         # read from stdin, write to stdout
    # bin/fsqlf.exe input_file              # read from file, write to stdout
    # bin/fsqlf.exe input_file output_file  # use files for reading and writing
    # bin/fsqlf.exe --help                  # info on more options

Usage in Linux is equivalent, just without the extension '.exe'
Formatting configuration can be changed in 'formatting.conf'


4.2 Usage of graphical user interface
----
Go to directory 'bin/' (this is needed so 'wx_fsqlf.exe' would be started in the same folder where 'fsqlf.exe' is located)
In Windows execute 'wx_fsqlf.exe'
In Linux execute 'wx_fsqlf'

When it starts, just fill your code into the formatter window and press 'Format'
Formating configuration can be changed in 'formatting.conf'



5. Project site
----
Most recent source code can be found at sourceforge.
Project site address http://sourceforge.net/projects/fsqlf/
Source can also be forked at github https://github.com/dnsmkl/fsqlf



* * *


6. APPENDIX A - wxWidgets installation
====

I use Debian, so I describe what I did. Your system probably is different, but maybe this will at least give you an idea in which direction to go.

Install for normal compilation
----
Install WxWidgets
(http://wiki.wxwidgets.org/Installing_and_configuring_under_Ubuntu)

    #!/bin/bash
    # apt-get install libwxbase3.0-dev libwxgtk3.0-dev
    # cd /usr/include
    # ls | grep wx
    # ln -sv wx-3.0/wx wx

Install for cross-compiling windows binaries on linux
----
Install mingw (cross) compiler

    #!/bin/bash
    # apt-get install mingw-w64

Download wxWidgets source from http://www.wxwidgets.org/
(some forums mention that sources provided by ubuntu/debian are not good,
after experiment it appeared that `apt-get source libwxbase3.0-dev`,
doesn't contain several (external?) source files used by makefile e.g. png.c)

Build wxWidgets with mingw

    #!/bin/bash
    # ./configure --prefix=/usr/i686-w64-mingw32 --host=i686-w64-mingw32 --build=`./config.guess` --enable-unicode --disable-shared
    # make
    # make install
    # cd /usr/i686-w64-mingw32/include
    # ln -sv wx-3.0/wx wx

References
----
This was done by following instructions found in  http://wiki.wxwidgets.org/Cross-Compiling_Under_Linux
with a little adjustment by after reading  http://forums.codeblocks.org/index.php?topic=7988.msg%msg_id%
and  http://old.nabble.com/mingwm10.dll-ts8920679.html

