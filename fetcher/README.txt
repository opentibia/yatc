This directory contains libs needed to compile data file fetcher.





NOTE I REMOVED DEVPAK ANYWAYS, ITS TOO BIG AND CAN EASILY GET OUTDATED.
wxWidgets devpak included here is bugged. Once you install it go here:

C:\Dev-Cpp\wxWidgets-2.8.6-msw-static\lib\wx\include\msw-ansi-release-static-2.8

You'll see file named "wx". Rename it into "setup.h". Make new folder called "wx".
Move "setup.h" into "wx".

You should have file called like this:

C:\Dev-Cpp\wxWidgets-2.8.6-msw-static\lib\wx\include\msw-ansi-release-static-2.8\wx\setup.h




Now, pay attention. This is what needs to be added to include list:
C:\Dev-Cpp\wxWidgets-2.8.6-msw-static\lib\wx\include\msw-ansi-release-static-2.8\
C:\Dev-Cpp\wxWidgets-2.8.6-msw-static\include\

and the linker list:

C:\Dev-Cpp\wxWidgets-2.8.6-msw-static\lib





ps this file assumes the root of mingw is C:\Dev-Cpp. Don't let this confuse you.


ivucica








PPS important! in the end i have not used this devpak. however it's here for devcpp users.
i switched to codeblocks when i started to work on fetcher. it works with 