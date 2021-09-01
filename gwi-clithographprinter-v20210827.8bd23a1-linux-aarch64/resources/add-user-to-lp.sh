#!/bin/bash

OS_NAME=`lsb_release -i -s`
GROUP=`stat -c '%G' /dev/usb/lp0 2>/dev/null || echo lp`

# Exit if not running on Kylin.
if [ "$OS_NAME" != "Kylin" ]; then
    echo "Error: Script made for Kylin only."
    exit 1
fi

# Add $USER to group $GROUP (lp by default).
echo "> Checking groups settings..."
if [[ "`groups`" = *" $GROUP"* ]]; then
    echo "> $USER already present in group $GROUP."
else
    echo "> Adding $USER to group $GROUP..."
    sudo usermod -a -G $GROUP $USER
    echo "> Reboot needed to take into account groups update."
    read -p "> Reboot now ? (y/n) " REBOOT
    if [ "$REBOOT" = "y" ]; then
        sudo reboot
    fi
fi

