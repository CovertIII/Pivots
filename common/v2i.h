#include <cmath>
#include <cfloat>
#include <cstddef>
#include <limits>

//These functions were taken from the b2Math.h file of the Box2D project and then rename and tweaked for my purposed


/// A 2D column vector.
struct v2i
{
	/// Default constructor does nothing (for performance).
	v2i() {}

	/// Construct using coordinates.
	v2i(float x, float y) : x(x), y(y) {}

	/// Set this vector to all zeros.
	void SetZero() { x = 0.0f; y = 0.0f; }

	/// Set this vector to some specified coordinates.
	void Set(float x_, float y_) { x = x_; y = y_; }
	
	v2i Get(void) { v2i i (x,y); return i; }

	/// Negate this vector.
	v2i operator -() const { v2i v; v.Set(-x, -y); return v; }

	/// Add a vector to this vector.
	void operator += (const v2i& v)
	{
		x += v.x; y += v.y;
	}
	
	/// Subtract a vector from this vector.
	void operator -= (const v2i& v)
	{
		x -= v.x; y -= v.y;
	}

	/// Multiply this vector by a scalar.
	void operator *= (float a)
	{
		x *= a; y *= a;
	}

	/// Get the length of this vector (the norm).
	float Length() const
	{
		return sqrtf(x * x + y * y);
	}

	/// Get the length squared. For performance, use this instead of
	/// v2i::Length (if possible).
	float LengthSquared() const
	{
		return x * x + y * y;
	}

	/// Convert this vector into a unit vector. Returns the length.
	float Normalize()
	{
		float length = Length();
		if (length < 0.01f)
		{
			return 0.0f;
		}
		float invLength = 1.0f / length;
		x *= invLength;
		y *= invLength;

		return length;
	}

	int x, y;
};

extern const v2i v2i_zero;

/// Perform the dot product on two vectors.
inline float v2iDot(const v2i& a, const v2i& b)
{
	return a.x * b.x + a.y * b.y;
}

/// Perform the cross product on two vectors. In 2D this produces a scalar.
inline float v2iCross(const v2i& a, const v2i& b)
{
	return a.x * b.y - a.y * b.x;
}

/// Perform the cross product on a vector and a scalar. In 2D this produces
/// a vector.
inline v2i v2iCross(const v2i& a, float s)
{
	return v2i(s * a.y, -s * a.x);
}

/// Perform the cross product on a scalar and a vector. In 2D this produces
/// a vector.
inline v2i v2iCross(float s, const v2i& a)
{
	return v2i(-s * a.y, s * a.x);
}

/// Add two vectors component-wise.
inline v2i operator + (const v2i& a, const v2i& b)
{
	return v2i(a.x + b.x, a.y + b.y);
}

/// Subtract two vectors component-wise.
inline v2i operator - (const v2i& a, const v2i& b)
{
	return v2i(a.x - b.x, a.y - b.y);
}

inline v2i operator * (float s, const v2i& a)
{
	return v2i(s * a.x, s * a.y);
}

inline bool operator == (const v2i& a, const v2i& b)
{
	return a.x == b.x && a.y == b.y;
}

inline float v2iDistance(const v2i& a, const v2i& b)
{
	v2i c = a - b;
	return c.Length();
}

inline float v2iDistanceSquared(const v2i& a, const v2i& b)
{
	v2i c = a - b;
	return v2iDot(c, c);
}