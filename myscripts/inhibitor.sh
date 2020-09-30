#!/bin/zsh
notify-send "Inhibitor" "Activated" &
while sleep 30 ; do xdotool keydown Shift_L keyup Shift_L ; done
