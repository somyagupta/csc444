#!/bin/sh

# we provide a small wrapper for Html Help Workshop because the Html HW returns 1 when
# it has compiled the help and the 'make' program then stops

hhc ../help/help.hhp

if [ $? == 0 ]
then 
	# there was an error
	exit 1
fi

# there was a success

exit 0
