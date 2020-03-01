#!/bin/sh
PID=$(pidof the64)
ADDR_START=$(cat /proc/$PID/maps | grep c64emu.rgl | tail -n 1 | cut -c10-17)


#./tc64monitor $PID $ADDR_START 125C5F4 A210
./tc64monitor $PID $ADDR_START 125f876 048A

