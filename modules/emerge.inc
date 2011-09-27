#!/bin/sh
# 'emerge' Module for gentoo-update.sh

emerge_run()
{
	if ! module_query_cmd tmpfs unmount; then
		run $*
	else
		# Special case for tmpfs enabled script
		if ! run --return $* && \
			ask "Executing '$*' failed! Do you want to unmount tmpfs?"; then
				module_cmd tmpfs unmount
				exit 1
		fi
	fi
}

emerge_do__init()
{
	emerge -V &>/dev/null || die "sys-apps/portage is not installed!"
	revdep-rebuild --help &>/dev/null || die "app-portage/gentoolkit is not installed!"
}

emerge_do_world()
{
	emerge_run emerge -DNuav @world
	pause
}

emerge_do_emptytree()
{
	print "Parsing empty-tree output..."
	local etoutput
	etoutput=$(emerge -ep world | sed -r -e '/^[^\[]/d' -e '/\[ebuild.+R.+\]/d' -e 's/^\[[^]]+\][[:space:]]+([^[:space:]]+).*$/=\1/')
	if [[ -z $etoutput ]]; then
		print "Nothing to update through empty-tree..."
	else
		emerge_run emerge -1 $etoutput
		pause
	fi
}

emerge_do_cleanup()
{
	run dispatch-conf
	run emerge --depclean
	emerge_run revdep-rebuild -i
	run eclean -d distfiles
}
