#!/bin/sh
# Special 'global' Module for gentoo-update.sh

GLOBAL_PRINT_PREFIX="[UpdateGentoo] "
MODULE_DIR="$(dirname $0)/modules"


# Load Module
load_module()
{
	run --silent source "$MODULE_DIR/$1.inc"
	# s_run exits the script if source errors out
	if module_query_cmd $1 _init; then
		# if _init exists, run it
		run --silent ${1}_do__init
	fi
}

# start module
module_cmd()
{
	local optional
	optional=0
	# check if this is an optional module
	if [[ "$1" == "--optional" ]]; then
		shift
		optional=1
	fi

	if [[ $# -ne 2 ]]; then
		print "$FUNCNAME error: invalid parameters!"
		exit 1
	else
		if type -t ${1}_do_${2} &>/dev/null; then
			${1}_do_${2}
		else
			if [[ optional -eq 0 ]]; then
				die "Non-optional function '${1}_do_${2}' not found!"
			fi
		fi
	fi
}

module_query_cmd()
{
	type -t ${1}_do_${2} &>/dev/null
	return $?
}

# print message
print() {
	echo "$GLOBAL_PRINT_PREFIX $*"
}

# pause script
pause() {
	read -p "$GLOBAL_PRINT_PREFIX Press any key to continue..."
}

# ask user a y/n question
ask() {
	local answer
	answer=""
	while [[ "$answer" != [YyNn] ]]; do
		read -p "$GLOBAL_PRINT_PREFIX  $* [y/n] " answer
	done
	if [[ "$answer" == [Yy] ]]; then
		return 0
	fi
	return 1
}

# Run a command
run() {
	local silent
	silent=0
	# check if this is an optional module
	if [[ "$1" == "--silent" ]]; then
		shift
		silent=1
	fi

	if [[ $# -eq 0 ]]; then
		die "Invalid $FUNCNAME parameters!"
	fi

	[[ $silent -ne 1 ]] && print "Executing '$*'.."
	$* || die "Command '$*' failed!"
}

die()
{
	print "Error: $*"
	exit 1
}
