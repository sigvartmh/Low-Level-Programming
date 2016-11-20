CC=ptxdist
GAME=game
DRIVER=driver-gamepad


.phony: game
game:
	ptxdist clean $(GAME) 
	ptxdist compile $(GAME)
	ptxdist targetinstall $(GAME)
	ptxdist image root.romfs
	ptxdist test flash-rootfs

.phony: driver
driver:
	ptxdist clean $(DRIVER) 
	ptxdist compile $(DRIVER)
	ptxdist targetinstall $(DRIVER)
	ptxdist targetinstall kernel
	ptxdist image root.romfs
	ptxdist test flash-rootfs
