a snake game written entirely in c using the ncurses library.

dependencies (debian and derivatives)
```shell
apt install glibc
```

dependencies (arch and derivatives)
```shell
pacman -S glibc ncurses
```

to build a binary
```shell
git clone https://github.com/flippyshoe1/csnake.git
cd csnake
make
```

i made this program to learn c, cant promise i handled memory correctly here (i havent ran into any issues).
feel free to do whatever with this, but i wouldnt advice running this on a professional enviroment (unless you wish to tackle memory leaks (if there are any)).
