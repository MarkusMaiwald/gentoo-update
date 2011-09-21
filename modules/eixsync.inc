#!/bin/sh
# 'eixsync' Module for gentoo-update.sh

eixsync_do__init()
{
	eix --version &>/dev/null || die "eix not found."
}

eixsync_do_run()
{
	run eix-sync
}
