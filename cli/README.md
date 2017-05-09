# YATC CLI

This directory includes a small CLI-only login-only version of the client,
exercising the login protocol implementation.

There are some stub versions of classes that are expected by the various parts
of the networking code, which have had all their graphics code stripped out.
(We do not need to read the .spr files in a CLI client. We do not need .dat
parsing in a login-only client.)

The client still requires SDL, as some uses of SDL (e.g. SDL_GetTicks()) have
not been eliminated.

The client is just for experimentation, and thus no care was taken to make it
autoconf'igurable.
