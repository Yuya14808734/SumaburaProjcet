#include "Calculation.h"
#include <math.h>

//--------------------------
// 2D Vector
//--------------------------

const CVector2 CVector2::m_Right = CVector2(1.0f,0.0f);
const CVector2 CVector2::m_Up = CVector2(0.0f, 1.0f);
const CVector2 CVector2::m_Zero = CVector2(0.0f, 0.0f);

CVector2::CVector2()
{
	x = y = 0.0f;
}

CVector2::CVector2(const CVector2& v)
{
	x = v.x;
	y = v.y;
}

CVector2::CVector2(const DirectX::XMFLOAT2& v)
{
	this->x = v.x;
	this->y = v.y;
}

CVector2::CVector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

CVector2::CVector2(int x, int y)
{
	this->x = static_cast<float>(x);
	this->y = static_cast<float>(x);
}


// assignment operators
CVector2& CVector2::operator += (const CVector2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

CVector2& CVector2::operator -= (const CVector2& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

CVector2& CVector2::operator *= (float r)
{
	x *= r;
	y *= r;
	return *this;
}

CVector2& CVector2::operator /= (float r)
{
	if (r != 0.0f) {
		x /= r;
		y /= r;
	}
	return *this;
}

// unary operators
CVector2 CVector2::operator + () const
{
	return *this;
}

CVector2 CVector2::operator - () const
{
	return CVector2(-x, -y);
}

// binary operators
CVector2 CVector2::operator + (const CVector2& v) const
{
	return CVector2(x + v.x, y + v.y);
}

CVector2 CVector2::operator - (const CVector2& v) const
{
	return CVector2(x - v.x, y - v.y);
}

CVector2 CVector2::operator * (float r) const
{
	return CVector2(x * r, y * r);
}

CVector2 CVector2::operator / (float r) const
{
	if (r != 0.0f)
		return CVector2(x / r, y / r);
	return *this;
}

CVector2 operator * (float r, const CVector2& v)
{
	return CVector2(r * v.x, r * v.y);
}

bool CVector2::operator == (const CVector2& v) const
{
	return (x == v.x && y == v.y);
}

bool CVector2::operator != (const CVector2& v) const
{
	return (x != v.x || y != v.y);
}

float CVector2::cross(CVector2& v) const
{
	return (x * v.y - y * v.x);
}

float CVector2::distance(CVector2& v) const
{
	return CVector2(x - v.x, y - v.y).length();
}

float CVector2::dot(CVector2& v) const
{
	return (x * v.x + y * v.y);
}

float CVector2::length() const
{
	return sqrtf(x * x + y * y);
}

CVector2 CVector2::normalize() const
{
	float l = length();
	if (l != 0.0f)
		return CVector2(x / l, y / l);
	return CVector2(0.0f, 1.0f);
}

CVector2 CVector2::rotateRadius(float Radius)
{
	CVector2 temp;
	temp.x = cosf(Radius) * x - sinf(Radius) * y;
	temp.y = sinf(Radius) * x + cosf(Radius) * y;
	return temp;
}

CVector2 CVector2::rotateAngle(float Angle)
{
	return rotateRadius(Angle * 3.14f / 180.0f);
}

CVector2 CVector2::Abs() const
{
	return CVector2(
		fabsf(x),
		fabsf(y)
	);
}

const CVector2& CVector2::GetUp()
{
	return m_Up;
}

const CVector2& CVector2::GetRight()
{
	return m_Right;
}

const CVector2& CVector2::GetZero()
{
	return m_Zero;
}

//--------------------------
// 3D Vector
//--------------------------
// constructors

const CVector3 CVector3::m_Forward = CVector3(0.0f,0.0f,1.0f);
const CVector3 CVector3::m_Right = CVector3(1.0f, 0.0f, 0.0f);
const CVector3 CVector3::m_Up = CVector3(0.0f, 1.0f, 0.0f);
const CVector3 CVector3::m_Zero = CVector3(0.0f, 0.0f, 0.0f);

CVector3::CVector3()
{
	x = y = z = 0.0f;
}

CVector3::CVector3(const CVector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

CVector3::CVector3(const CVector3* p)
{
	if (p) {
		x = p->x;
		y = p->y;
		z = p->z;
	}
	else {
		x = y = z = 0.0f;
	}
}

CVector3::CVector3(const DirectX::XMFLOAT3& v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
}

CVector3::CVector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

CVector3::CVector3(int x, int y, int z)
{
	this->x = static_cast<float>(x);
	this->y = static_cast<float>(y);
	this->z = static_cast<float>(z);
}

CVector3& CVector3::operator =(const DirectX::XMFLOAT3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

// assignment operators
CVector3& CVector3::operator += (const CVector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

CVector3& CVector3::operator -= (const CVector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

CVector3& CVector3::operator *= (float r)
{
	x *= r;
	y *= r;
	z *= r;
	return *this;
}

CVector3& CVector3::operator /= (float r)
{
	if (r != 0.0f) {
		x /= r;
		y /= r;
		z /= r;
	}
	return *this;
}

// unary operators
CVector3 CVector3::operator + () const
{
	return *this;
}

CVector3 CVector3::operator - () const
{
	return CVector3(-x, -y, -z);
}

// binary operators
CVector3 CVector3::operator + (const CVector3& v) const
{
	return CVector3(x + v.x, y + v.y, z + v.z);
}

CVector3 CVector3::operator - (const CVector3& v) const
{
	return CVector3(x - v.x, y - v.y, z - v.z);
}

CVector3 CVector3::operator * (float r) const
{
	return CVector3(x * r, y * r, z * r);
}

CVector3 CVector3::operator / (float r) const
{
	if (r != 0.0f) {
		return CVector3(x / r, y / r, z / r);
	}
	return *this;
}

bool CVector3::operator == (const CVector3& v) const
{
	return (x == v.x && y == v.y && z == v.z);
}

bool CVector3::operator != (const CVector3& v) const
{
	return (x != v.x || y != v.y || z != v.z);
}

CVector3 CVector3::cross(CVector3 v) const
{
	return CVector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

float CVector3::distance(CVector3 v) const
{
	return CVector3(x - v.x, y - v.y, z - v.z).length();
}

float CVector3::dot(CVector3 v) const
{
	return (x * v.x + y * v.y + z * v.z);
}

float CVector3::length() const
{
	return sqrtf(x * x + y * y + z * z);
}

CVector3 CVector3::normalize() const
{
	float l = length();
	if (l != 0.0f)
		return CVector3(x / l, y / l, z / l);
	return CVector3(0.0f, 0.0f, 1.0f);
}

CVector3 CVector3::Abs() const
{
	return CVector3(
	fabsf(x),
	fabsf(y),
	fabsf(z)
	);
}

const CVector3& CVector3::GetForward()
{
	return m_Forward;
}

const CVector3& CVector3::GetUp()
{
	return m_Up;
}

const CVector3& CVector3::GetRight()
{
	return m_Right;
}

const CVector3& CVector3::GetZero()
{
	return m_Zero;
}

//--------------------------
// CQuaternion
//--------------------------

CQuaternion::CQuaternion()
	:CQuaternion(0.0f,0.0f,0.0f,1.0f)
{
}

CQuaternion::CQuaternion(float x, float y, float z, float w)
{
	DirectX::XMFLOAT4 f = DirectX::XMFLOAT4(x, y, z, w);
	v = DirectX::XMLoadFloat4(&f);
}

CQuaternion::CQuaternion(const DirectX::XMFLOAT4& f)
	:CQuaternion(f.x,f.y,f.z,f.w)
{
	
}

CQuaternion CQuaternion::operator*(const CQuaternion& q)
{
	CQuaternion r_q;
	r_q.v = DirectX::XMQuaternionMultiply(this->v, q.v);
	return r_q;
}

CQuaternion CQuaternion::operator*=(const CQuaternion& q)
{
	this->v = DirectX::XMQuaternionMultiply(this->v, q.v);
	return *this;
}

CVector3 CQuaternion::RotateVector(const CVector3& vec)
{
	CQuaternion vq(vec.x,vec.y,vec.z,0.0f);
	CQuaternion cq(-vec.x,-vec.y,-vec.z,0.0f);
	CQuaternion mq = cq * vq * (*this);
	DirectX::XMFLOAT4 f4;
	DirectX::XMStoreFloat4(&f4, this->v);

	return DirectX::XMFLOAT3(f4.x, f4.y, f4.z);
}

DirectX::XMFLOAT4 CQuaternion::GetValue()
{
	DirectX::XMFLOAT4 f;
	DirectX::XMStoreFloat4(&f, this->v);
	return f;
}

CQuaternion CQuaternion::AngleAxis(const CVector3& Axis, float Angle)
{
	CQuaternion r_q;
	float radian = Angle * 3.14159265359f / 180.0f;
	DirectX::XMVECTOR axis_v = DirectX::XMLoadFloat3(&Axis.f);

	r_q.v = DirectX::XMQuaternionRotationAxis(axis_v, radian);

	return r_q;
}

CQuaternion CQuaternion::RadianAxis(const CVector3& Axis, float Radian)
{
	CQuaternion r_q;
	DirectX::XMVECTOR axis_v = DirectX::XMLoadFloat3(&Axis.f);

	r_q.v = DirectX::XMQuaternionRotationAxis(axis_v, Radian);

	return r_q;
}

CQuaternion CQuaternion::LookAt(const CVector3& Pos)
{
	return CQuaternion();
}
