#pragma once

class MathHelper {
public:
	static float Clamp(float value, float min, float max) {
		if (value < min)
			return min;
		if (value > max)
			return max;
		return value;
	}

	static float Pow(double base, double ex) {
		// power of 0
		if (ex == 0) {
			return 1;
			// negative exponenet
		} else if (ex < 0) {
			return 1 / pow(base, -ex);
			// even exponenet
		} else if ((int)ex % 2 == 0) {
			float half_pow = pow(base, ex / 2);
			return half_pow * half_pow;
			//integer exponenet
		} else {
			return base * pow(base, ex - 1);
		}
	}
};