#!/bin/sh
# tmpfs-emerge

##
## parameter memory
##
DO_SYNC=0
DO_ET=0
DO_PERLCLEAN=0


##
## Global functions
##
if ! source "$(dirname $0)/modules/global.inc"; then
	echo "FATAL ERROR: Loading global functions failed!!"
	exit 1
fi

##
## Optional modules
##
load_module tmpfs

##
## Optional: mount tmpfs if it's not mounted already
##
module_cmd tmpfs mount


##
## Run emerge
##
if ! run --silent --return emerge $* && \
		! ask "Executing 'emerge $*' failed! Do you wan't to unmount tmpfs?"; then
	exit 1
fi

##
## Optional: unmount tmpfs
##
module_cmd tmpfs unmount
