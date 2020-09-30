#!/bin/sh

# A dwm_bar function to read the battery level and status
# Joe Standring <git@joestandring.com>
# GNU GPLv3

dwm_battery () {
    # Change BAT1 to whatever your battery is identified as. Typically BAT0 or BAT1
    CHARGE=$(cat /sys/class/power_supply/BAT0/capacity)
    STATUS=$(cat /sys/class/power_supply/BAT0/status)

    printf "%s" "$SEP1"
    if [ "$IDENTIFIER" = "unicode" ]; then
        if [ "$STATUS" = "Charging" ]; then
            printf "  %s%% %s" "$CHARGE" "$STATUS" #🔌
    	elif [ $CHARGE -lt 75 ] && [ $CHARGE -gt 50  ]; then
	    printf "  %s%% %s" "$CHARGE" "$STATUS"
	elif [ $CHARGE -lt 50 ] && [ $CHARGE -gt 25  ]; then
	    printf "  %s%% %s" "$CHARGE" "$STATUS"
	elif [ $CHARGE -lt 25 ] && [ $CHARGE -gt 10  ]; then
	    printf "  %s%% %s" "$CHARGE" "$STATUS"
	elif [ $CHARGE -lt 10 ]; then
	    printf "  %s%% %s" "$CHARGE" "$STATUS"
        else
            printf "  %s%% %s" "$CHARGE" "$STATUS" #🔋
        fi
    else
        printf "BAT %s%% %s" "$CHARGE" "$STATUS"
    fi
    printf "%s" #"$SEP2"
}

dwm_battery

