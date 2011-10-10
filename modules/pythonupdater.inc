#!/bin/sh
# 'pythonupdater' Module for gentoo-update.sh

pythonupdater_update_queued=0

pythonupdater_do__init()
{
	python-updater -V &>/dev/null || die "app-admin/python-updater isn't installed!"
}

pythonupdater_do_updatecheck()
{
	local python_update_output
	python_update_output=$(emerge -1p =python-2* =python-3* | sed -r -e '/^[^\[]/d' -e '/\[ebuild.+R.+\]/d')
	if [[ -z $python_update_output ]]; then ## Debug
		print "No python update sighted.."
		pythonupdater_update_queued=0
	else
		print "python UPDATE QUEUED!!"
		pythonupdater_update_queued=1
	fi
}

pythonupdater_do_run()
{
	run python-updater
}

