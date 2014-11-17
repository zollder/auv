/*
 * SensorData.h
 */

#ifndef sensordata_h
#define sensordata_h

	template <typename T> struct SensorData
	{
		/** Constructor. */
		SensorData(T xValue, T yValue, T zValue)
		{
			x = xValue;
			y = yValue;
			z = zValue;
		}

		/** Instance variables. */
		T x, y, z;
	};

#endif
