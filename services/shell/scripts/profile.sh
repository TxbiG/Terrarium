# User profile
source /services/shell/scripts/env.sh

if [ -x /usr/sbin/terrarium-lumina ]; then
    export WAYLAND_DISPLAY=wayland-0
fi
