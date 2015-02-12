/*
 * Config.h
 */

#ifndef config_h
#define config_h

/* ----------------------------------------------------
 * PWM configuration parameters
 ------------------------------------------------------*/

/** File path components. */
#define PATH "/sys/devices/ocp.3/"
#define PERIOD "/period"
#define DUTY "/duty"
#define POLARITY "/polarity"
#define RUN "/run"

#define PWM_MODULE_1_ID 1	// motors 1 & 2
#define PWM_MODULE_2_ID 2	// motors 3 & 4
#define PWM_MODULE_3_ID 3	// motors 5 & 6

#define P8_13 "pwm_test_P8_13.12"
#define P8_19 "pwm_test_P8_19.13"
#define P9_14 "pwm_test_P9_14.14"
#define P9_16 "pwm_test_P9_16.15"
#define P9_29 "pwm_test_P9_29.16"
#define P9_31 "pwm_test_P9_31.17"

/** Conversion constant (ns) */
#define CONVERSION_CONST 1000000000

/** Default period value (Hz) */
#define DEFAULT_PERIOD_HZ 50
#define BASE_PERIOD 50	// Hz

/* Default forward/reverse duty cycle limits (ns) */
#define NEUTRAL 1500000
#define FORWARD_MIN 1545000
#define FORWARD_MAX 2000000
#define REVERSE_MIN 1455000
#define REVERSE_MAX 1000000

#endif
