#!/bin/bash                                                             
#  Title: validate.sh                                                 #
#  Description: Script used to execute examples for VM validation     #
#  Developer: Mauricio De Carvalho                                    #
#  email: mauricio.m.c@gmail.com                                      #
#  Last updated: 08/06/2016                                           #
#                                                                     #
#######################################################################
## Get all .bin binaries to execute on the VM
binfiles=(`ls ./bin | grep ".bin" | grep -v task | sort`)

## Execute each one sequentially and search for an 'X'
for i in ${binfiles[*]}; do
	X=`./vm.o bin/${i} | grep 'X' | wc -l`

	# If there isnt an X, then specific .bin has failed, and VM 
	# must be corrected
	## Else, specific .bin has passed test
	if (( X == 0 )); then
		echo "${i} has error"
	else
		echo "${i} passed"
	fi

### If all binaries has showed passed, then VM is theoretically well 
### developed. However, there still lack some binaries to test binop
### decoded value.
 
done
