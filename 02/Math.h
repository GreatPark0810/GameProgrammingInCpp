#pragma once

#include <cmath>
#include <limits>

namespace Math {
    const float Pi = 3.1415926535f;
    const float TwoPi = Pi * 2.0f;
    const float PiOver2 = Pi / 2.0f;
    const float Infinity = std::numeric_limits<float>::infinity();
    const float NegInfinity = -std::numeric_limits<float>::infinity();

    inline float DegreesToRadians(float degrees) {
        return degrees * Pi / 180.0f;
    }

    inline float RadiansToDegrees(float radians) {
        return radians * 180.0f / Pi;
    }

    inline bool NearZero(float val, float epsilon = 0.001f) {
        if (fabs(val) <= epsilon) return true;
        else return false;
    }

    template <typename T>
    T Max(const T& a, const T& b) {
        return (a < b ? b : a);
    }

    template <typename T>
    T Min(const T& a, const T& b) {
        return (a < b ? a : b);
    }

    template <typename T>
    T Clamp(const T& val, const T& lower, const T& upper) {
        return Min(upper, Max(lower, val));
    }

    inline float Abs(float value) {
		return fabs(value);
	}

	inline float Cos(float angle) {
		return cosf(angle);
	}

	inline float Sin(float angle) {
		return sinf(angle);
	}

	inline float Tan(float angle) {
		return tanf(angle);
	}

	inline float Acos(float value) {
		return acosf(value);
	}
	
	inline float Atan2(float y, float x) {
		return atan2f(y, x);
	}

	inline float Cot(float angle) {
		return 1.0f / Tan(angle);
	}

	inline float Lerp(float a, float b, float f) {
		return a + f * (b - a);
	}

	inline float Sqrt(float value) {
		return sqrtf(value);
	}
	
	inline float Fmod(float numer, float denom) {
		return fmod(numer, denom);
	}
}

class Vector2 {
    public:
        float x;
        float y;

        static const Vector2 Zero;
        static const Vector2 UnitX;
        static const Vector2 UnitY;
        static const Vector2 NegUnitX;
        static const Vector2 NegUnitY;

        Vector2() : x(0.0f), y(0.0f) {}
        explicit Vector2(float x, float y) : x(x), y(y) {}

        void SetValue(float _x, float _y) {
            x = _x;
            y = _y;
        }

        friend Vector2 operator+(const Vector2& a, const Vector2& b) {
            return Vector2(a.x + b.x, a.y + b.y);
        }

        friend Vector2 operator-(const Vector2& a, const Vector2& b) {
            return Vector2(a.x - b.x, a.y - b.y);
        }

        // 요소별 곱
        friend Vector2 operator*(const Vector2& a, const Vector2& b) {
            return Vector2(a.x * b.x, a.y * b.y);
        }

        // 스칼라 곱
        friend Vector2 operator*(const Vector2& v, float scalar) {
            return Vector2(v.x * scalar, v.y * scalar);
        }

        friend Vector2 operator*(float scalar, const Vector2& vec) {
            return Vector2(vec.x * scalar, vec.y * scalar);
        }

        Vector2& operator*=(float scalar) {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        Vector2& operator+=(const Vector2& right) {
            x += right.x;
            y += right.y;
            return *this;
        }

        Vector2& operator-=(const Vector2& right) {
            x -= right.x;
            y -= right.y;
            return *this;
        }

        // 벡터 크기의 제곱
        float LengthSq() const {
            return (x*x + y*y);
        }

        // 벡터의 크기
        float Length() const {
            return (Math::Sqrt(LengthSq()));
        }

        // 벡터의 크기 1로 정규화
        void Normalize() {
            float length = Length();
            x /= length;
            y /= length;
        }

        // 인자로 주어진 벡터 정규화
        static Vector2 Normalize(const Vector2& vec) {
            Vector2 temp = vec;
            temp.Normalize();
            return temp;
        }

        // 벡터 내적
        static float Dot(const Vector2& a, const Vector2& b) {
            return (a.x * b.x + a.y * b.y);
        }

        // 선형보간된 새로운 벡터 반환
        static Vector2 Lerp(const Vector2& a, const Vector2& b, float f) {
            return Vector2(a + f * (b-a));
        }

        // 반사벡터 반환
        // 반사벡터 R = V + 2 * (-V dot N) * N
        static Vector2 Reflect(const Vector2& v, const Vector2& n) {
            return v - 2.0f * Vector2::Dot(v, n) * n;
        }
};