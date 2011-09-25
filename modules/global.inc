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
	# parse parameters
	local params
	params=( $(get_long_parameters $*) )
	shift ${#params[*]}

	if [[ $# -ne 2 ]]; then
		print "$FUNCNAME error: invalid parameters!"
		exit 1
	else
		if type -t ${1}_do_${2} &>/dev/null; then
			${1}_do_${2}
		else
			if ! [[ "${params[*]}" =~ "optional" ]]; then
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

get_long_parameters()
{
	local params
	params=""
	while [[ "${1:0:2}" == "--" ]]; do
		params="${params}${1:2} "
		shift
	done
	echo $params
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
	# parse parameters
	local params
	params=( $(get_long_parameters $*) )
	shift ${#params[*]}

	if [[ $# -eq 0 ]]; then
		die "Invalid $FUNCNAME parameters!"
	fi

	[[ "${params[*]}" =~ "silent" ]] || print "Executing '$*'.."
	if [[ "${params[*]}" =~ "return" ]]; then
		$*
		return $?
	else
		$* || die "Executing '$*' failed!"
	fi
}

die()
{
	print "Error: $*"
	exit 1
}
