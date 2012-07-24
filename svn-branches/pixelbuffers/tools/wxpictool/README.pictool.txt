   pictool
 ANSI C converter for Tibia's PIC files

(c) 2007-2008 Ivan Vucica
Part of OpenTibia project
-------------------------

1) Why a new converter?

Many people are familiar with existence of Trooper's editor for PIC files. 
However this editor is somewhat crudely written; for the simple task it should
perform, it uses way too many libraries and features of C++. What I mean is,
Boost is unnecessary; ANSI C is surely supported on even more platforms than,
for example, Boost.

Then, it is interesting that Trooper disregarded portability to 16-bit and 
64-bit platforms (the latter being more and more prominent on the market).
For example, he declared the Tibia.pic's signature as "unsigned version;"
which could mean anything between a 16 bit integer and a 64 bit integer,
some day perhaps even more.

Another thing is a problem, and that is the licensing issue. Trooper has not
made a statement that his editor is free software (either GPL or another 
license). So, I decided to write a converter which will clearly be under the
GPLv2 license.

Most other converters are dependent on the version with which Tibia.pic is
shipping with. This converter is not dependent on it.

Of course, credits go to Trooper for writing the original converter using
which I could figure out the format quickly and painlessly. Thanks, Trooper!

2) Instructions

This is a command line program. To use it, you must already know how to
use your operating system's command line. This is a too wide subject, you
will have to learn this by yourself.

To read instructions, run pictool without arguments.
