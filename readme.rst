=============
gentoo-update
=============

gentoo-update is a set of simple wrappers for emerge and few other useful gentoo maintenance tools. Point is to make system updating process little bit more streamlined.

gentoo-update has following dependencies:
- app-portage/eix
- app-portage/gentoolkit
- app-admin/perl-cleaner

This thing comes with two main scripts, update-gentoo.sh and tmpfs-emerge.sh.

----------------
update-gentoo.sh
----------------

update-gentoo.sh runs emerge -DNuav world, depclean, perl-cleaner, revdep-rebuild etc. It also has following command-line parameters:

======  ===============  ==========================================================================
  -s      --sync         syncs portage tree using eix-sync
  -e      --emptytree    gathers additional updatable packages by parsing emerge -e world output
  -p      --perlclean    forces perl-cleaner (normally script should detect when perl is updated)
======  ===============  ==========================================================================

Script also has an optional (enabled by default) tmpfs module which can be used to direct emerge temporary build directory to a tmpfs mount. Remember that openoffice etc. should not be even tried to compile inside tmpfs for it's ~10G building size. Use per-package PORTAGE_TMPDIR settings for that [#per-package-tmpdir]_. tmpfs directory and maximum size settings are located at the start of modules/tmpfs.inc.

.. [#per-package-tmpdir] http://blog.jolexa.net/2011/09/16/gentoo-per-package-portage_tmpdir-settings

---------------
tmpfs-emerge.sh
---------------

tmpfs-emerge.sh is just a wrapper to use the tmpfs module with basic emerge.

