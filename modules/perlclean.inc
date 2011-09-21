#!/bin/sh
# 'perlclean' Module for gentoo-update.sh

perlclean_update_queued=0

perlclean_do__init()
{
	perl-cleaner -V &>/dev/null || die "app-admin/perl-cleaner isn't installed!"
}

perlclean_do_updatecheck()
{
	local perl_update_output
	perl_update_output=$(emerge -p perl | sed -r -e '/^[^\[]/d' -e '/\[ebuild.+R.+\]/d')
	if [[ -z $perl_update_output ]]; then ## Debug
		print "No perl update sighted.."
		perlclean_update_queued=0
	else
		print "PERL UPDATE QUEUED!!"
		perlclean_update_queued=1
	fi
}

perlclean_do_clean()
{
	run perl-cleaner --all
	if $(ask "Do you want to cleanup leftover perl files?"); then
		local perl_files
		perl_files=$(perl-cleaner --leftovers | tail -n +7)
		if [[ -z $perl_files ]]; then
			print "No leftover perl files found."
		else
			run --silent rm -v $(echo $perl_files | xargs)
		fi
	fi
}

