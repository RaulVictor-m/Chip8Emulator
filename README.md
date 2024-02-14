# Chip-8

This is a chip8 emulator, which was made to learning about how emulators work in general. The implementation is still missing a few ajustments on timings and also has no sound, But overall it is working with correctly for most games.

It was built on top of the [OLCPIXELGAMEENGINE library].(https://github.com/OneLoneCoder/olcPixelGameEngine)

## Building

There is currently only linux support for building the project, but its dependencies should be completely cross plataform just check the [olc wiki](https://github.com/OneLoneCoder/olcPixelGameEngine/wiki) and follow the guide on how to build it on windows if you are interested.

As for people on linux just use:

```console
$ make build
```

## Usage

When you start the program it will prompt you for the ROM you want to run, currently there is nothing to help you do that, you will have to manually type the path by hand, but once you do it will just run the game or whatever ROM you put into it, and hopefully you don't encounter any bugs, but if you do you can leave an issue.
