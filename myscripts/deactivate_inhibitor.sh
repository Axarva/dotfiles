#!/bin/zsh
notify-send "Inhibitor" "Deactivated"
kill -15 $(pgrep foo.sh)
