#! /bin/bash
# description: auto start watchdog for auv

process="auv"

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
        echo 0 > /sys/devices/ocp.3/pwm_test_P8_13.14/run
        echo 0 > /sys/devices/ocp.3/pwm_test_P8_19.15/run
        echo 0 > /sys/devices/ocp.3/pwm_test_P9_14.12/run
        echo 0 > /sys/devices/ocp.3/pwm_test_P9_16.13/run
        echo 0 > /sys/devices/ocp.3/pwm_test_P9_29.16/run
        echo 0 > /sys/devices/ocp.3/pwm_test_P9_31.17/run

        echo 0 > /sys/devices/ocp.3/pwm_test_P8_13.14/duty
        echo 0 > /sys/devices/ocp.3/pwm_test_P8_19.15/duty
        echo 0 > /sys/devices/ocp.3/pwm_test_P9_14.12/duty
        echo 0 > /sys/devices/ocp.3/pwm_test_P9_16.13/duty
        echo 0 > /sys/devices/ocp.3/pwm_test_P9_29.16/duty
        echo 0 > /sys/devices/ocp.3/pwm_test_P9_31.17/duty

}

while [ 1 ]; do
        check_process
        if [ $? -eq 0 ]
        then
                #logger -t "AUV_WATCHDOG"  "Process is not running"
                stop_pwm
        fi
        sleep 15
done
exit 0

