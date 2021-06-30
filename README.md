# UESTC network login
[![example workflow name](https://github.com/ehds/uestc_network_manager/workflows/C/C++%20CI/badge.svg)](https://github.com/ehds/uestc_network_manager/actions)

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

### kDomain:
When you are in laboratory:set `kDomain=@dx-uestc`.
When you are in dorm and using chinatelecom:set `kDomain=@dx`
If you are in dorm and using and using chinamobile, you can check your domain when you manually login and the `kDomain` will display on the success website link. If you find the correct chinamobile `kDomain`, welcome to contact us.

### ac_id
This parameter is determined by the region supplying the network.
When you are in laboratory, it probably set to 1
When you are in dorm, it probably set to 3
you also can check your domain when you manually login and the `ac_id` will display on the success website link.

Enable this service and start it.
```
systemctl enable uestcnetwork
systemctl start uestcnetwork
```

**Windows、 MacOS and OpenWrt comming soon**
