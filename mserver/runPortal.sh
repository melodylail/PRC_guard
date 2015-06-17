#!/bin/bash
JETTY_RUN='.'
JETTY_PID='./jetty.pid'
JAVA_OPTIONS='-Xms1024m -Xmx1024m -Djetty.class.path=conf'

COMMAND=$1

case $COMMAND in
    start | stop)
    ;;
    *)
        echo "Usage: ./runPortal.sh {start | stop}"
        exit -1
        ;;
esac

case $COMMAND in
    start)
        source bin/jetty.sh start
        ;;
    stop)
        source bin/jetty.sh stop
        ;;
esac
