#!/bin/bash

# This script is a part of RPC & TI-RPC Test Suite
# (c) 2007 BULL S.A.S.
# Please refer to RPC & TI-RPC Test Suite documentation.
# More details at http://nfsv4.bullopensource.org/doc/rpc_testsuite.php

# This scripts launch everything needed to test RPC & TI-RPC
# Never try to launch alone, run "script_run.sh" instead
# Note : this script could be in more than one copy depending on what
#        tests series you want to run

# By C. LACABANNE - cyril.lacabanne@bull.net
# creation : 2007-06-18 revision : 2007-06-19

# **********************
# *** INITIALISATION ***
# **********************

# simple tests suite identification
TESTSUITENAME="TIRPC toplevel mt domain"
TESTSUITEVERS="0.1"
TESTSUITEAUTH="Cyril LACABANNE"
TESTSUITEDATE="2007-06-18"
TESTSUITECOMM=""

TESTSERVER_1_PATH="tirpc_svc_6"
TESTSERVER_1_BIN="tirpc_svc_6.bin"
TESTSERVER_1=$SERVERTSTPACKDIR/$TESTSERVER_1_PATH/$TESTSERVER_1_BIN

export TESTSERVER_1_PATH
export TESTSERVER_1_BIN
export TESTSERVER_1

# check if tests run locally or not
# if not, logs directory will be change to remote directory
if [ "$LOCALIP" != "$CLIENTIP" ]
then
	LOGDIR=/tmp/$LOGDIR
	if [ $VERBOSE -eq 1 ]
	then
		echo " - log dir changes to client log dir : "$LOGDIR # debug !
	fi
fi

# *****************
# *** PROCESSUS ***
# *****************

echo "*** Starting Tests Suite : "$TESTSUITENAME" (v "$TESTSUITEVERS") ***"

#-- start TIRPC MT Server for that following tests series
$REMOTESHELL $SERVERUSER@$SERVERIP "$TESTSERVER_1 $PROGNUMBASE $NBTHREADPROCESS"&

#-- start another instance of TIRPC MT server for simple API call type test
$REMOTESHELL $SERVERUSER@$SERVERIP "$TESTSERVER_1 $PROGNUMNOSVC $NBTHREADPROCESS"&

# wait for server creation and initialization
sleep $SERVERTIMEOUT

### SCRIPT LIST HERE !!! ###
./$SCRIPTSDIR/tirpc_toplevel_clnt_call.mt.sh

#-- Cleanup
$REMOTESHELL $SERVERUSER@$SERVERIP "killall -9 "$TESTSERVER_1_BIN

#-- Unreg all procedure
for ((a=PROGNUMNOSVC; a < `expr $PROGNUMNOSVC + $NBTHREADPROCESS` ; a++))
do
	$REMOTESHELL $SERVERUSER@$SERVERIP "$SERVERTSTPACKDIR/cleaner.bin $a"
done
for ((a=PROGNUMBASE; a < `expr $PROGNUMBASE + $NBTHREADPROCESS` ; a++))
do
	$REMOTESHELL $SERVERUSER@$SERVERIP "$SERVERTSTPACKDIR/cleaner.bin $a"
done

# ***************
# *** RESULTS ***
# ***************
