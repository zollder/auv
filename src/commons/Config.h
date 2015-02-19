/*
 * Config.h
 */

#ifndef config_h
#define config_h

/* ----------------------------------------------------
 * IMU configuration parameters
 ------------------------------------------------------*/
#define IMU_THREAD_ID 1
#define IMU_INTERVAL 0.02	// 20ms

/* ----------------------------------------------------
 * Heading controller parameters
 ------------------------------------------------------*/
#define HC_THREAD_ID 4
#define HC_INTERVAL 0.1	// 100 ms

#define YAW_KP 0.25
#define YAW_KI 0
#define YAW_KD 0

/* ----------------------------------------------------
 * Altitude controller parameters
 ------------------------------------------------------*/
#define AC_THREAD_ID 6
#define AC_INTERVAL 0.05	// 50 ms

 //(TODO: replace with constants when configured)

// altimeter controller coefficients
#define ALT_KP 0.25
#define ALT_KI 0
#define ALT_KD 0

// pitch controller coefficients
#define PITCH_KP 0.25
#define PITCH_KI 0
#define PITCH_KD 0

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

// module 1
#define P9_29 "pwm_test_P9_29.16"
#define P9_31 "pwm_test_P9_31.17"

// module 2
#define P9_14 "pwm_test_P9_14.12"
#define P9_16 "pwm_test_P9_16.13"

// module3
#define P8_13 "pwm_test_P8_13.14"
#define P8_19 "pwm_test_P8_19.15"

/** Conversion constant (ns) */
#define CONVERSION_CONST 1000000000

/** Default period value (Hz) */
#define PWM_PERIOD_HZ 50
#define BASE_PERIOD 50	// Hz

/* Default forward/reverse duty cycle limits (ns) */
#define NEUTRAL 1500000
#define FORWARD_MIN 1545000
#define FORWARD_MAX 2000000
#define REVERSE_MIN 1455000
#define REVERSE_MAX 1000000

#endif
