all: shell
	echo "done"

shell: SM_shell.c
	$gcc SM_shell.c -o SM_shell

