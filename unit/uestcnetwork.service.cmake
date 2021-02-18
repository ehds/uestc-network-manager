[Unit]
Description=UESTC Network Login
Documention=
After=network.target

[Service]
Type=${SYSTEMD_SERVICE_TYPE}
ExecStart=${CMAKE_INSTALL_PREFIX}/bin/uestcnetwork
ExecReload=/bin/kill -s USR1 $MAINPID
Restart=on-failure
AmbientCapabilities=CAP_NET_BIND_SERVICE

[Install]
WantedBy=multi-user.target
