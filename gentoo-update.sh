#!/bin/sh
# Semi-automated gentoo-updater

##
## parameter memory
##
DO_SYNC=0
DO_UPGRADE=0
DO_PERLCLEAN=0
DO_PYTHONUPDATER=0

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
load_module pythonupdater

##
## Optional modules
##
load_module tmpfs


##
## Check parameters
##
while [[ $# -gt 0 ]]; do
	case "$1" in
		-u|-U|--upgrade)
			DO_UPGRADE=1
			;;
		-s|-S|--sync)
			DO_SYNC=1
			;;
		-p|-P|--perlclean)
			DO_PERLCLEAN=1
			;;
		-y|-y|--pythonupdater)
			DO_PYTHONUPDATER=1
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
## Check if we're going to update python (if expicit python updater has not been selected)
##
if [[ $DO_PYTHONUPDATER -eq 0 ]]; then
	module_cmd pythonupdater updatecheck
fi


##
## Optional: mount tmpfs if it's not mounted already
##
module_cmd --optional tmpfs mount

##
## Update or Upgrade @world
##
if [[ $DO_UPGRADE -ge 1 ]]; then
	module_cmd emerge upgrade
else
	module_cmd emerge update
fi

##
## Perl-cleaner
##
if [[ $DO_PERLCLEAN -ge 1 || $perlclean_update_queued -ge 1 ]]; then
	module_cmd perlclean clean
fi

##
## Python-updater
##
if [[ $DO_PYTHONUPDATER -ge 1 || $pythonupdater_update_queued -ge 1 ]]; then
	module_cmd pythonupdater run
fi


##
## Cleanup and system check
##
module_cmd emerge cleanup

##
## Optional: unmount tmpfs
##
module_cmd --optional tmpfs unmount
