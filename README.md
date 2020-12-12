# UESTC network login
![example workflow name](https://github.com/ehds/uestc_network_manager/workflows/C/C++%20CI/badge.svg)

# Linux
## Build
```
mkdir build
cd build && cmake ..
make
make install
```

## Usage
Edit `/etc/uestc_networkmanager/user.conf`, fill your information.

Enable this service and start it.
```
systemctl enable uestcnetwork
systemctl start uestcnetwork
```

**Windows and MacOS comming soon**
