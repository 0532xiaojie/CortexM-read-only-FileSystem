My very basic read-only file system for cortexM devices
==============

This is an easy example/unit test for my read-only file system implementation. You could open and parse text files with POSIX conform functions.

Hardware
--------------
STM32F4 Discovery Board -> CortexM4 with FPU
UART2 TX at Pin A2

License
--------------
GPLv2

Software
--------------
System Workbench (Eclipse based)
STM CubeMX

Introduction
--------------
Convert a file (text mode only) to a C array with this website: http://fuxx.h1.ru/bin2c/index.en.php
Copy it to the header file and expand the struct in flash_fs.c.