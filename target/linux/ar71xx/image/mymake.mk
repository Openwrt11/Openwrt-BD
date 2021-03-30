define LegacyDevice/TENDA-O3_V2_8M
  DEVICE_TITLE := MYMAKE TENDA-O3_V2 (8MB flash)
  DEVICE_PACKAGES := rssileds kmod-leds-gpio \
    luci luci-app-commands kmod-fs-ext4 kmod-fs-nfs-v3 kmod-fs-nfs-v4 luci-app-nfs 	
    bridge luci-proto-relay luci-app-filetransfer aircrack-ng luci-app-ramfree luci-app-watchcat kmod-macvlan luci-app-mwan3 \
    ppp-mod-pptp luci-app-uhttpd kmod-nls-cp936 kmod-nls-utf8 zram-swap e2fsprogs mount-utils nfs-utils screen
endef
LEGACY_DEVICES += TENDA-O3_V2_8M

define LegacyDevice/TENDA-O3_V2_16M
  DEVICE_TITLE := MYMAKE TENDA-O3_V2 (16MB flash)
  DEVICE_PACKAGES := rssileds kmod-leds-gpio \
    luci luci-app-commands kmod-fs-ext4 kmod-fs-nfs-v3 kmod-fs-nfs-v4 luci-app-nfs 	
    bridge luci-proto-relay luci-app-filetransfer aircrack-ng luci-app-ramfree luci-app-watchcat kmod-macvlan luci-app-mwan3 \
    ppp-mod-pptp luci-app-uhttpd kmod-nls-cp936 kmod-nls-utf8 zram-swap e2fsprogs mount-utils nfs-utils screen
endef
LEGACY_DEVICES += TENDA-O3_V2_16M


