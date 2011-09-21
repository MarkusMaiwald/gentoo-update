#!/bin/sh
# 'emerge' Module for gentoo-update.sh

emerge_do_world()
{
	run emerge -DNuav @world
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
		run emerge -1 $etoutput
		pause
	fi
}

emerge_do_cleanup()
{
	run dispatch-conf
	run emerge --depclean
	run revdep-rebuild -i
	run eclean -d distfiles
}
