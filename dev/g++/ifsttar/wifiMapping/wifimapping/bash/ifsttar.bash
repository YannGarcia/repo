############################################################################
#!/bin/sh
set +e

# Setup internal variables
#INSTALL_PATH=/usr/local
INSTALL_PATH=/home/debian
PATH_BIN=${INSTALL_PATH}/bin
BIN=${PATH_BIN}/wifimapping
PATH_LIB=${INSTALL_PATH}/lib
PATH_LOG=/var/log/ifsttar
LOG=${PATH_LOG}/logs
# Backup current dir.
PWD=`pwd`
# Process command
case "$1" in
    start)
	sudo LD_LIBRARY_PATH=${PATH_LIB} ${BIN} --autoMode false > ${LOG} 2>&1 3>&1
        # Check system
	stty sane
	sleep 5
	ipcs -mq
	ps aux | egrep 'iperfmon|wifimapping|guimon|gpsmon|tshark|accmon|tempmon|rtcmon|aggreg' | grep -v egrep
	;;
    
    stop)
        sudo pkill wifimapping
	;;
    
    status)
	ipcs -mq
	ps aux | egrep 'iperfmon|wifimapping|guimon|gpsmon|tshark|accmon|tempmon|rtcmon|aggreg' | grep -v egrep
	;;
    *)
        echo 'Usage: ifsttar {start|stop|status}'
        exit 1 # Exit with error
    ;;
esac # End of 'case' statement
# Go back to original dir.
cd ${PWD}
# Exit with no error
exit 0

