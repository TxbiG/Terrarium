# Launch compositor
/usr/sbin/terrarium-lumina &
COMPOSITOR_PID=$!

sleep 1 # allow backend init

# Launch Nova desktop shell components.
/usr/sbin/terrarium-nova --once &

# Launch shell
/usr/bin/terrarium-shell

wait $COMPOSITOR_PID
