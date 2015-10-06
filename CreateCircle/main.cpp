/*
	Written by Markus Tillman.

	Two functions for generating the points on a circle in triangle-strip order.
	The first function (CreateCircle()) mirrors half of the circle's points and thus iterates half the number of points on the circle.
	The second function (CreateCircle2()) further takes advantage of the symmetry to iterate a quarter of the points on the circle.

	Instead of generating several points on the circle, which would require several calls to sin and cos,
	a single point is created and then rotated along the circle.
	Rotating a point (x,y) around the origin of a circle with radius of 1 with an angle of a,
	the point ends up at (u,v) where
	u = cos(a) * x  + sin(a) * y
	v = sin(a) * -x + cos(a) * y
*/
#pragma warning (disable : 4514 4710 4711) // Disable inlined functions warnings.
#if defined (DEBUG) || (_DEBUG)
	#include <crtdbg.h>
#endif
#include <assert.h>
#include <cmath> // std::acos.
#include <iostream> // std::cout.
#include <cstdlib> // strtol().
using namespace std;

static const long double PI = acos(-1.0L); // Let acos() return the number of digits for pi the processor supports.
static const long double PI_MUL_2 = PI * 2.0L;

template<class IntegralType, typename = std::enable_if<std::is_integral<IntegralType>::value>::type>
bool IsOdd(IntegralType number)
{
	return number % 2 == 1; // The compiler (if clever enough) will use a bitwise "and" to determine if the number is odd since the expression is boolean.
}

/*
	Calculates the points on a circle assuming a radius of 1 and position (0,0).
	Input: Array of any floating-point data type to hold the points.
	Output: Points on the circle in triangle-strip fashion ordered by the "clockwise" parameter.
*/
template<class FloatType, typename = std::enable_if<std::is_floating_point<FloatType>::value>::type>
void CreateCircle(unsigned int nrOfPoints, FloatType* pointsOut, bool clockwise = true)
{
	if (pointsOut == nullptr)
	{
		throw std::invalid_argument("Pointer to output array cannot be null!");
	}

	FloatType angle = clockwise ? FloatType(PI_MUL_2) / FloatType(nrOfPoints) : FloatType(-PI_MUL_2) / FloatType(nrOfPoints);
	FloatType cosIncr = cos(angle);
	FloatType sinIncr = sin(angle);
	auto halfNrOfPoints = nrOfPoints / 2;
	FloatType xNew;
	FloatType x;
	FloatType y;
	unsigned long long arrayIndex = 2;
	if (IsOdd(nrOfPoints))
	{
		// Iterating from top to bottom on right half circle.
		// First (top) point.
		x = pointsOut[0] = 0.0f;
		y = pointsOut[1] = 1.0f;

		// Other points.
		for (decltype(halfNrOfPoints) i = 0; i < halfNrOfPoints; ++i)
		{
			xNew = cosIncr * x + sinIncr * y;
			y = sinIncr * -x + cosIncr * y;
			x = xNew;

			pointsOut[arrayIndex] = x;
			pointsOut[arrayIndex + 1] = y;
			// Mirror point.
			pointsOut[arrayIndex + 2] = -x;
			pointsOut[arrayIndex + 3] = y;
			arrayIndex += 4;
		}
		assert(arrayIndex == nrOfPoints * 2);
	}
	else
	{
		// Iterating from right to left on the bottom half circle.
		// First (right-most) point.
		x = pointsOut[0] = 1.0f;
		y = pointsOut[1] = 0.0f;

		// Other points.
		for (decltype(halfNrOfPoints) i = 0; i < halfNrOfPoints - 1; ++i)
		{
			xNew = cosIncr * x + sinIncr * y;
			y = sinIncr * -x + cosIncr * y;
			x = xNew;

			pointsOut[arrayIndex] = x;
			pointsOut[arrayIndex + 1] = y;
			// Mirror point.
			pointsOut[arrayIndex + 2] = x;
			pointsOut[arrayIndex + 3] = -y;
			arrayIndex += 4;
		}

		// Last (left-most) point.
		pointsOut[arrayIndex] = -1.0f;
		pointsOut[arrayIndex + 1] = 0.0f;
		assert(arrayIndex + 2 == nrOfPoints * 2);
	}
}

/*
	Calculates the points on a circle assuming a radius of 1 and position (0,0).
	Input: Array of any floating-point data type to hold the points.
	Output: Points on the circle in triangle-strip fashion ordered by the "clockwise" parameter.
*/
template<class FloatType, typename = std::enable_if<std::is_floating_point<FloatType>::value>::type>
void CreateCircle2(unsigned int nrOfPoints, FloatType* pointsOut, bool clockwise = true)
{
	if (pointsOut == nullptr)
	{
		throw std::invalid_argument("Pointer to output array cannot be null!");
	}

	FloatType angle = clockwise ? FloatType(PI_MUL_2) / FloatType(nrOfPoints) : FloatType(-PI_MUL_2) / FloatType(nrOfPoints);
	FloatType cosIncr = cos(angle);
	FloatType sinIncr = sin(angle);
	int halfNrOfPoints = static_cast<int>(nrOfPoints / 2);
	FloatType xNew;
	FloatType x;
	FloatType y;
	unsigned long long arrayIndex = 2;
	if (IsOdd(nrOfPoints))
	{
		// Iterating from top to bottom on right half circle.
		// First (top) point.
		x = pointsOut[0] = 0.0f;
		y = pointsOut[1] = 1.0f;

		// Other points.
		for (decltype(halfNrOfPoints) i = 0; i < halfNrOfPoints; ++i)
		{
			xNew = cosIncr * x + sinIncr * y;
			y = sinIncr * -x + cosIncr * y;
			x = xNew;

			pointsOut[arrayIndex] = x;
			pointsOut[arrayIndex + 1] = y;
			// Mirror point.
			pointsOut[arrayIndex + 2] = -x;
			pointsOut[arrayIndex + 3] = y;
			arrayIndex += 4;
		}
		assert(arrayIndex == nrOfPoints * 2);
	}
	else
	{
		// Iterating from right to left on the bottom half circle.
		// First (right-most) point.
		x = pointsOut[0] = 1.0f;
		y = pointsOut[1] = 0.0f;

		// Other points.
		for (decltype(halfNrOfPoints) i = 0; i < halfNrOfPoints / 2; ++i)
		{
			xNew = cosIncr * x + sinIncr * y;
			y = sinIncr * -x + cosIncr * y;
			x = xNew;

			pointsOut[arrayIndex] = x;
			pointsOut[arrayIndex + 1] = y;
			// Mirror point.
			pointsOut[arrayIndex + 2] = x;
			pointsOut[arrayIndex + 3] = -y;
			arrayIndex += 4;
		}
		auto startIndex = halfNrOfPoints % 2 ? halfNrOfPoints - 2 : halfNrOfPoints - 3;
		for (auto i = startIndex; i > 0; i -= 2)
		{
			x = pointsOut[i * 2];
			y = pointsOut[i * 2 + 1];

			pointsOut[arrayIndex] = -x;
			pointsOut[arrayIndex + 1] = y;
			// Mirror point.
			pointsOut[arrayIndex + 2] = -x;
			pointsOut[arrayIndex + 3] = -y;
			arrayIndex += 4;
		}

		// Last (left-most) point.
		pointsOut[arrayIndex] = -1.0f;
		pointsOut[arrayIndex + 1] = 0.0f;
		assert(arrayIndex + 2 == nrOfPoints * 2);
	}
}

int main(int argc, char* argv[])
{
#if defined (DEBUG) || (_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	unsigned long nrOfPoints = 5;
	if (argc > 1)
	{
		nrOfPoints = strtoul(argv[1], nullptr, 10);
	}

	float* points = nullptr;
	try
	{
		points = new float[nrOfPoints * 2];
	}
	catch (std::bad_alloc& e)
	{
		cout << "Failed to allocate memory using " << nrOfPoints << " points; " << e.what() << endl;
		return 1;
	}

	cout << "Circle with " << nrOfPoints << " points:" << endl;
	CreateCircle(nrOfPoints, points);
	for (unsigned long i = 0; i < nrOfPoints * 2; i += 2)
	{
		cout << points[i] << "," << points[i + 1] << endl;
	}
	delete[] points;

	return 0;
}


