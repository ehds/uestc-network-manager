[Unit]
Description=UESTC Network Login
Documention=
After=network.target

[Service]
Type=${SYSTEMD_SERVICE_TYPE}
ExecStart=${CMAKE_INSTALL_PREFIX}/bin/uestcnetwork
ExecReload=/bin/kill -s USR1 $MAINPID
WatchdogSec=${WATCHDOG_SEC}
Restart=always
User=${SERVICE_USER}
Group=${SERVICE_GROUP}
AmbientCapabilities=CAP_NET_BIND_SERVICE

[Install]
WantedBy=multi-user.target
