include $(TOPDIR)/rules.mk

# Name, version and release number
# The name and version of your package are used to define the variable to point to the build directory of your package: $(PKG_BUILD_DIR)
PKG_NAME:=uestcnetwork
PKG_VERSION:=1.0
PKG_RELEASE:=1

# Source settings (i.e. where to find the source codes)
# This is a custom variable, used below
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/ehds/uestc_network_manager.git
include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk #如果使用cmake，则必须增加此行

# Package definition; instructs on how and where our package will appear in the overall configuration menu ('make menuconfig')
define Package/uestcnetwork
	DEPENDS:=+libstdcpp +libopenssl 
	SECTION:=examples
	CATEGORY:=Examples
	TITLE:=Hello, World!
endef

# Package description; a more verbose description on what our package does
define Package/uestcnetwork/description
	A simple "Hello, world!" -application.
endef

# Package preparation instructions; create the build directory and copy the source code. 
# The last command is necessary to ensure our preparation instructions remain compatible with the patching system.
define Build/Prepare
		mkdir -p $(PKG_BUILD_DIR)
		cp -r $(SOURCE_DIR)/* $(PKG_BUILD_DIR)
		$(Build/Patch)
endef

# Package install instructions; create a directory inside the package to hold our executable, and then copy the executable we built previously into the folder
define Package/uestcnetwork/install
		$(INSTALL_DIR) $(1)/usr/bin
		$(INSTALL_BIN) $(PKG_BUILD_DIR)/uestcnetwork $(1)/usr/bin
endef

# This command is always the last, it uses the definitions and variables we give above in order to get the job done
$(eval $(call BuildPackage,uestcnetwork))
