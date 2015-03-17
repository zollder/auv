#! /bin/bash
# chkconfig: 345 99 10
# description: auto start watchdog for auv

process="firefox"

check_process() {
    if pgrep -n $process
    then
        return 1
    else
        return 0
  fi
}

stop_pwm() {
    logger -t "AUV_WATCHDOG" "Stopping PWM"
}

case "$1" in
    'start')
        ##while [ 1 ]; do
        check_process
        if [ $? -eq 0 ]
        then
            echo "Process is not running"
            stop_pwm
            exit 1
        else
            echo "Process is Running"
        fi
        sleep 1
        ;;
        ##done
    'stop')
        pkill -n watch-auv.sh
        ;;
    *)
        echo $"Usage: $0 {start|stop}"
        exit 1
esac
exit 0
