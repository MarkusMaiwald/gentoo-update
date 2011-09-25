#!/bin/sh
# 'tmpfs' Module for gentoo-update.sh

tmpfs_path="/var/tmp/portage"
tmpfs_size="1G"

tmpfs_do__init()
{
	grep -q tmpfs /proc/filesystems || die "No tmpfs support detected!"
}

# Check if tmpfs is already mounted
tmpfs_mounted()
{
	mount | grep -q "^tmpfs.*${tmpfs_path}[[:space:]]*type[[:space:]]*tmpfs"
	return $?
}

tmpfs_do_mount()
{
	# Doesn't work with per-package tmpdir
	#export PORTAGE_TMPDIR="$tmpfs_path"
	if ! tmpfs_mounted; then
		if 	mount -t tmpfs -o size=$tmpfs_size tmpfs $tmpfs_path; then
			print "tmpfs with maximum size of $tmpfs_size has been mounted successfully."
		else
			die "Mounting tmpfs failed!"
		fi
	else
		print "tmpfs already mounted, using it."
	fi
}

tmpfs_do_unmount()
{
	if tmpfs_mounted ; then
		if ! pgrep -f /usr/bin/emerge &>/dev/null || (( $(pgrep -fc $(dirname $0)) = 1 )); then
			umount $tmpfs_path || die "tmpfs umount failed!"
			print "tmpfs was unmounted successfully."
		else
			print "Not unmounting tmpfs as other processes might be using it."
		fi
	fi
}
