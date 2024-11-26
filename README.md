# nsh
A small shell I built in C. Inspired by https://brennan.io/2015/01/16/write-a-shell-in-c/.

## build
1. Clone the repository.
```
git clone git@github.com:Nathan5563/nsh.git
```

2. Build the project files.
```
make
```

3. Run the executable.
```
./nsh.exe
```

## built-in functions
The shell currently supports the following functions:
```
help   -  Usage: 'help'       -   displays information about the built-in functions.
cd     -  Usage: 'cd [args]'  -   change the present working directory to the path specified by args.
exit   -  Usage: 'exit'       -   exits the shell.
```
