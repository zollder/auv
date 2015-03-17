/*
 * Config.h
 */

#ifndef config_h
#define config_h

/* ----------------------------------------------------
 * Escape sequence
 ------------------------------------------------------*/
#define ESC 27
#define DEPTH_COMPENSATOR -5.0

/* ----------------------------------------------------
 * thread IDs
 ------------------------------------------------------*/
#define IMU_THREAD_ID 1
#define DMU_SH_TIMER 3		/* DMU's single-shot timer ID */
#define HC_THREAD_ID 4
#define HM_THREAD_ID 5
#define AC_THREAD_ID 6
#define MC_THREAD_ID 7
#define SERVER_THREAD_ID 8
#define CLIENT_U3_FRONT_ID 9
#define CLIENT_U3_BOTTOM_ID 10
#define RETRY_TIMER_ID 11

/* ----------------------------------------------------
 * timer intervals
 ------------------------------------------------------*/
#define IMU_INTERVAL 0.02	// 20ms
#define DMU_SH_TIMER_DELAY 0.01	// 10ms

#define CLIENT_U3_FRONT_INTERVAL 0.5
#define CLIENT_U3_BOTTOM_INTERVAL 1.0
#define CLIENT_RETRY_INTERVAL 0.1

#define HC_INTERVAL 0.05	// 50 ms
#define MC_INTERVAL 0.1	// 100 ms
#define AC_INTERVAL 0.05	// 100 ms
#define HM_INTERVAL 0.1	// 100 ms

/* ----------------------------------------------------
 * Socket server/client parameters
 ------------------------------------------------------*/
#define BBB_IP "192.168.0.10"
#define U3_BOTTOM_IP "192.168.0.11"
#define U3_FRONT_IP "192.168.0.12"

#define PORT 5000
#define CLIENTS 2

#define RETRY_COUNTER 10

/* ----------------------------------------------------
 * Controller parameters
 ------------------------------------------------------*/
// yaw controller coefficients
#define YAW_KP 0.80
#define YAW_KI 0.00
#define YAW_KD 0.00

// altimeter controller coefficients
#define ALT_KP 0.50
#define ALT_KI 0.50
#define ALT_KD 0

// pitch controller coefficients
#define PITCH_KP 0.80
#define PITCH_KI 0.00
#define PITCH_KD 0.00

/* ----------------------------------------------------
 * PWM configuration parameters
 ------------------------------------------------------*/

/** File path components. */
#define PATH "/sys/devices/ocp.3/"
#define PERIOD "/period"
#define DUTY "/duty"
#define POLARITY "/polarity"
#define RUN "/run"

#define PWM_MODULE_1_ID 1	// motors 1(f) & 2(r)	altitude and level control
#define PWM_MODULE_2_ID 2	// motors 3(f) & 4(r) 	heading and drift control
#define PWM_MODULE_3_ID 3	// motors 5(l) & 6(r) 	speed control

// module 1
#define P9_29 "pwm_test_P9_29.16"
#define P9_31 "pwm_test_P9_31.17"

// module 2
#define P9_14 "pwm_test_P9_14.14"
#define P9_16 "pwm_test_P9_16.15"

// module3
#define P8_13 "pwm_test_P8_13.12"
#define P8_19 "pwm_test_P8_19.13"

/** Conversion constant (ns) */
#define CONVERSION_CONST 1000000000

/** Default period value (Hz) */
#define PWM_PERIOD_HZ 50
#define BASE_PERIOD 50	// Hz

/* Default forward/reverse duty cycle limits (ns) */
#define NEUTRAL 1500000
#define FORWARD_MIN 1545000
#define FORWARD_MAX 1900000
#define REVERSE_MIN 1455000
#define REVERSE_MAX 1100000

#endif
