tc64monitor
---

tc64monitor is a simple tool to peek/poke the (emulated) C64 memory space of [theC64](https://retrogames.biz/thec64).

It works by invoking the ```process_vm_readv``` / ```process_vm_writev``` syscalls on the virtual memory regions of "the64" process containing the emulated C64 memory space. The helper script runtc64mon.sh automatically finds the PID of "the64" process, recovers the address of the relevant memory pages and invokes the tc64monitor main executable.

## Usage:


1. Copy ```bin/tc64monitor``` and ```bin/runtc64mon.sh``` on a USB flash drive.
2. Connect the flash drive to theC64 and open a root shell (See [here](https://thec64community.online/thread/76/2020-thec64-maxi-modding-games) for info on how to do that)
3. Boot theC64 in classic mode and enter the following commands:

```
cd /mnt
./runtc64mon.sh
```

4. Type ```?``` for the list of supported commands

## Test it

Boot theC64 in classic mode and wait for the basic prompt. Open the monitor and enter the following:

```
:$05F0 08050C0C0F
```

The word "HELLO" should appear at the center of the screen.


## Known problems

Not all the C64 memory space can be correctly accessed. For example, ```:$D020 10``` won't change the screen border color as it should. If you want to contribute please feel free to fork and send a pull request.

## License

```
    tc64monitor
    Copyright (C) 2020  Filippo Bergamasco

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
```