#!/bin/sh
# Semi-automated gentoo-updater

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
## Mandatory modules
##
load_module emerge
load_module eixsync
load_module perlclean


##
## Optional modules
##
load_module tmpfs


##
## Check parameters
##
while [[ $# -gt 0 ]]; do
	case "$1" in
		-e|-E|--emptytree)
			DO_ET=1
			;;
		-s|-S|--sync)
			DO_SYNC=1
			;;
		-p|-P|--perlclean)
			DO_PERLCLEAN=1
			;;
	esac
	shift
done


##
## if to do a sync
##
if [[ $DO_SYNC -ge 1 ]]; then
	module_cmd eixsync run
fi

##
## Check if we're going to update perl (if expicit perl clean has not been selected)
##
if [[ $DO_PERLCLEAN -eq 0 ]]; then
	module_cmd perlclean updatecheck
fi

##
## Optional: mount tmpfs if it's not mounted already
##
module_cmd --optional tmpfs mount

##
## Update @world
##
module_cmd emerge world

##
## If to do an empty tree parse
##
if [[ $DO_ET -ge 1 ]]; then
	module_cmd emerge emptytree
fi

##
## Perl-cleaner
##
if [[ $DO_PERLCLEAN -ge 1 || $perlclean_update_queued -ge 1 ]]; then
	module_cmd perlclean clean
fi

##
## Cleanup and system check
##
module_cmd emerge cleanup

##
## Optional: unmount tmpfs
##
module_cmd --optional tmpfs unmount
