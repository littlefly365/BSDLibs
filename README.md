# nlibc
nlibc is an experimental port of the NetBSD libc (and other basic libs) for Linux.

# Whats available?
- support for x86_64
- Optimized functions for x86_64
- The crt* objects from NetBSD
- Malloc and friends (jemalloc)
- argc, argv and envp supported
- BSD extensions

# Achievements
- Build and run commands from NetBSD like stat, chmod, kill, uname...

# Goals
- Build gcc and other GNU projects
- Build a significant part of the NetBSD userland
- Create a linux distribution using BSDLibC
