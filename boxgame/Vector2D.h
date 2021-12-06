#pragma once
#include <iostream>
#include <String>

template<class T>
class V2D_Generic
{
public:
	T x = 0;
	T y = 0;
	V2D_Generic() : x(0), y(0) {}
	V2D_Generic(T _x, T _y) : x(_x), y(_y) {}
	V2D_Generic(const V2D_Generic& v) : x(v.x), y(v.y) {}
	bool operator== (const V2D_Generic& v2d) const { return (this->x == v2d.x && this->y == v2d.y); }
	bool operator!= (const V2D_Generic& v2d) const { return (this->x != v2d.x || this->y != v2d.y); }
	V2D_Generic operator+ (const V2D_Generic& v2d) const { return V2D_Generic(this->x + v2d.x, this->y + v2d.y); }
	V2D_Generic operator+ (T& v2d) const { return V2D_Generic(this->x + v2d, this->y + v2d); }
	V2D_Generic operator- (const V2D_Generic& v2d) const { return V2D_Generic(this->x - v2d.x, this->y - v2d.y); }
	V2D_Generic operator- (T& v2d) const { return V2D_Generic(this->x - v2d, this->y - v2d); }
	V2D_Generic operator* (const V2D_Generic& v2d) const { return V2D_Generic(this->x * v2d.x, this->y * v2d.y); }
	V2D_Generic operator* (T& v2d) const { return V2D_Generic(this->x * v2d, this->y * v2d); }
	V2D_Generic operator/ (const V2D_Generic& v2d) const { return V2D_Generic(this->x / v2d.x, this->y / v2d.y); }
	V2D_Generic operator/ (T& v2d) const { return V2D_Generic(this->x / v2d, this->y / v2d); }
	V2D_Generic operator+= (const V2D_Generic& v2d) { this->x += v2d.x; this->y += v2d.y; return *this; }
	V2D_Generic operator+= (T& v2d) { this->x += v2d; this->y += v2d; return *this; }
	V2D_Generic operator-= (const V2D_Generic& v2d) { this->x -= v2d.x; this->y -= v2d.y; return *this; }
	V2D_Generic operator-= (T& v2d) { this->x -= v2d; this->y -= v2d; return *this; }
	V2D_Generic operator*= (const V2D_Generic& v2d) { this->x *= v2d.x; this->y *= v2d.y; return *this; }
	V2D_Generic operator*= (T& v2d) { this->x *= v2d; this->y *= v2d; return *this; }
	V2D_Generic operator/= (const V2D_Generic& v2d) { this->x /= v2d.x; this->y /= v2d.y; return *this; }
	V2D_Generic operator/= (T& v2d) { this->x /= v2d; this->y /= v2d; return *this; }
	const std::string str() const { return std::string("(") + std::to_string(this->x) + ", " + std::to_string(this->y) + ")"; }
	friend std::ostream& operator<< (std::ostream& os, const V2D_Generic& v2d) { os << v2d.str(); return os; }
};

typedef V2D_Generic<int32_t> vi2d;
typedef V2D_Generic<uint32_t> vu2d;
typedef V2D_Generic<float> vf2d;
typedef V2D_Generic<double> vd2d;