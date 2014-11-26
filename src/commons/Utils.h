/*
 * Vector.h
 *
 *  Created on: 04.10.2013
 *      Author: tuuzdu
 */

#ifndef utils_h
#define utils_h

//-----------------------------------------------------------------------------------------
/** Utils class implementation.
 *  Provides a collection of helper methods for vectors and matrices operations. */
//-----------------------------------------------------------------------------------------
class Utils
{
	public:

		//-----------------------------------------------------------------------------------------
		/** Calculates and returns a dot product of two vectors. */
		//-----------------------------------------------------------------------------------------
		float calculateDotProduct(float v1[3], float v2[3])
		{
			float op = 0;
			for (int c=0; c<3; c++)
				op += v1[c] * v2[c];

			return op;
		}

		//-----------------------------------------------------------------------------------------
		/** Calculates cross product of two vectors and save the result in the 3rd vector. */
		//-----------------------------------------------------------------------------------------
		void calculateCrossProduct(float out[3], float v1[3], float v2[3])
		{
			out[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
			out[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
			out[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
		}

		//-----------------------------------------------------------------------------------------
		/** Scales array members by specified scale factor. */
		//-----------------------------------------------------------------------------------------
		void scaleVector(float out[3], float in[3], float scale)
		{
			for(int c=0; c<3; c++)
				out[c] = in[c] * scale;
		}

		//-----------------------------------------------------------------------------------------
		/** Adds two vectors (arrays) and saves the result in the 3rd vector */
		//-----------------------------------------------------------------------------------------
		void addVector(float out[3], float in1[3], float in2[3])
		{
			for(int c=0; c<3; c++)
				out[c] = in1[c] + in2[c];
		}

		//-----------------------------------------------------------------------------------------
		/** Multiplies two 3x3 matrices. */
		//-----------------------------------------------------------------------------------------
		void multiplyMatrices(float a[3][3], float b[3][3], float mat[3][3])
		{
			float op[3];
			for(int x=0; x<3; x++)
			{
				for(int y=0; y<3; y++)
				{
					for(int w=0; w<3; w++)
						op[w] = a[x][w] * b[w][y];

					mat[x][y] = 0;
					mat[x][y] = op[0] + op[1] + op[2];
				}
			}
		}

		//-----------------------------------------------------------------------------------------
		/** Converts specified number to decimal. */
		//-----------------------------------------------------------------------------------------
		long convert_to_dec(float x)
		{
			return x*10000000;
		}
};

#endif
