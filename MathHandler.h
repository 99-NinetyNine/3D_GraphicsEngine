#pragma once
#include<vector>
#include <memory>

class Point
{
public:
	Point() :m_x(0), m_y(0)
	{
	}
	Point(int x, int y) :m_x(x), m_y(y)
	{
	}
	Point(const Point& point) :m_x(point.m_x), m_y(point.m_y)
	{
	}
	~Point()
	{
	}
public:
	int m_x = 0, m_y = 0;
};


class ORect
{
public:
	ORect() {}
	ORect(int width, int height) :width(width), height(height) {}
	ORect(int left, int top, int width, int height) :left(left), top(top), width(width), height(height) {}
	ORect(const ORect& rect) :left(rect.left), top(rect.top), width(rect.width), height(rect.height) {}
public:
	int width = 0, height = 0, left = 0, top = 0;
};


class OVec2
{
public:
	OVec2() :m_x(0), m_y(0)
	{
	}
	OVec2(float x, float y) :m_x(x), m_y(y)
	{
	}
	OVec2(const OVec2& vector) :m_x(vector.m_x), m_y(vector.m_y)
	{
	}
	void operator =(const OVec2& vec)
	{
		m_x = vec.m_x;
		m_y = vec.m_y;

	}
	OVec2 operator *(float num)
	{
		return OVec2(m_x * num, m_y * num);
	}

	OVec2 operator +(OVec2 vec)
	{
		return OVec2(m_x + vec.m_x, m_y + vec.m_y);
	}

	~OVec2()
	{
	}

public:
	float m_x, m_y;
};


class OVec3
{
public:
	OVec3() :m_x(0), m_y(0), m_z(0)
	{
	}
	OVec3(float x, float y, float z) :m_x(x), m_y(y), m_z(z)
	{
	}
	OVec3(float x) :m_x(x), m_y(x), m_z(x)
	{
	}
	OVec3(const OVec3& vector) :m_x(vector.m_x), m_y(vector.m_y), m_z(vector.m_z)
	{
	}

	static OVec3 lerp(const OVec3& start, const OVec3& end, float delta)
	{
		OVec3 v;
		v.m_x = start.m_x * (1.0f - delta) + end.m_x * (delta);
		v.m_y = start.m_y * (1.0f - delta) + end.m_y * (delta);
		v.m_z = start.m_z * (1.0f - delta) + end.m_z * (delta);
		return v;
	}

	OVec3 operator *(float num)
	{
		return OVec3(m_x * num, m_y * num, m_z * num);
	}

	OVec3 operator +(const OVec3& vec)
	{
		return	 OVec3(m_x + vec.m_x, m_y + vec.m_y, m_z + vec.m_z);
	}
	OVec3 operator - (const OVec3& vec)
	{
		return OVec3(m_x - vec.m_x, m_y - vec.m_y, m_z - vec.m_z);
	}
	OVec3 operator +=(const OVec3& vec)
	{
		m_x += vec.m_x;
		m_y += vec.m_y;
		m_z += vec.m_z;
		return *this;

	}
	OVec3 operator -=(const OVec3& vec)
	{
		m_x -= vec.m_x;
		m_y -= vec.m_y;
		m_z -= vec.m_z;
		return *this;
	}
	OVec3 operator =(const OVec3& vec)
	{
		m_x = vec.m_x;
		m_y = vec.m_y;
		m_z = vec.m_z;
		return *this;
	}
	float length() const
	{
		double res = (m_x * m_x) + (m_y * m_y) + (m_z * m_z);
		return (static_cast<float>((sqrt(res))));
	}

	static OVec3 normalize(const  OVec3& vec)
	{
		OVec3 res;
		double leng = (vec.m_x * vec.m_x) + (vec.m_y * vec.m_y) + (vec.m_z * vec.m_z);
		float len = static_cast<float>(sqrt(leng));
		if (!len)
			return OVec3();

		res.m_x = vec.m_x / len;
		res.m_y = vec.m_y / len;
		res.m_z = vec.m_z / len;

		return res;
	}

	static OVec3 cross(const  OVec3& v1, const  OVec3& v2)
	{
		OVec3 res;
		res.m_x = (v1.m_y * v2.m_z) - (v1.m_z * v2.m_y);
		res.m_y = (v1.m_z * v2.m_x) - (v1.m_x * v2.m_z);
		res.m_z = (v1.m_x * v2.m_y) - (v1.m_y * v2.m_x);
		return res;
	}
	static float dot(const OVec3& v1, const  OVec3& v2)
	{
		return v1.m_x * v2.m_x + v1.m_y * v2.m_y + v1.m_z * v2.m_z;
	}

	static OVec3 make_vec3(std::vector<float>& vertex)
	{
		OVec3 res;
		res.m_x = vertex[0];
		res.m_y = vertex[1];
		res.m_z = vertex[2];
		return res;
	}



	~OVec3()
	{
	}

public:
	float m_x, m_y, m_z;
};

class OVec4
{
public:
	OVec4() :m_x(0), m_y(0), m_z(0), m_w(0)
	{
	}
	OVec4(float x, float y, float z, float w) :m_x(x), m_y(y), m_z(z), m_w(w)
	{
	}
	OVec4(const OVec4& vec) :m_x(vec.m_x), m_y(vec.m_y), m_z(vec.m_z), m_w(vec.m_w)
	{
	}
	OVec4(const OVec3& vec) :m_x(vec.m_x), m_y(vec.m_y), m_z(vec.m_z), m_w(1.0)
	{
	}
	static OVec4 make_vec4(std::vector<float>& vec)
	{
		OVec4 res;
		res.m_x = vec[0];
		res.m_y = vec[1];
		res.m_z = vec[2];
		res.m_w = vec[3];
		return res;
	}

	void cross(OVec4& v1, OVec4& v2, OVec4& v3)
	{
		this->m_x = v1.m_y * (v2.m_z * v3.m_w - v3.m_z * v2.m_w) - v1.m_z * (v2.m_y * v3.m_w - v3.m_y * v2.m_w) + v1.m_w * (v2.m_y * v3.m_z - v2.m_z * v3.m_y);
		this->m_y = -(v1.m_x * (v2.m_z * v3.m_w - v3.m_z * v2.m_w) - v1.m_z * (v2.m_x * v3.m_w - v3.m_x * v2.m_w) + v1.m_w * (v2.m_x * v3.m_z - v3.m_x * v2.m_z));
		this->m_z = v1.m_x * (v2.m_y * v3.m_w - v3.m_y * v2.m_w) - v1.m_y * (v2.m_x * v3.m_w - v3.m_x * v2.m_w) + v1.m_w * (v2.m_x * v3.m_y - v3.m_x * v2.m_y);
		this->m_w = -(v1.m_x * (v2.m_y * v3.m_z - v3.m_y * v2.m_z) - v1.m_y * (v2.m_x * v3.m_z - v3.m_x * v2.m_z) + v1.m_z * (v2.m_x * v3.m_y - v3.m_x * v2.m_y));
	}
	OVec4 operator = (const OVec4& vec)
	{
		m_x = vec.m_x;
		m_y = vec.m_y;
		m_z = vec.m_z;
		m_w = vec.m_w;
		return *this;
	}
	~OVec4()
	{
	}

public:
	float m_x, m_y, m_z, m_w;
};



class OMat4
{
public:
	OMat4()
	{
		setIdentity();
	}
	OMat4(float x)
	{
		setIdentity();
	}
	static OMat4 make_mat4(std::vector<float>& m)
	{
		OMat4 res;
		int k = 0;
		for (int i = 0;i < 4;i++)
		{
			for (int j = 0;j < 4;j++)
			{
				res.m_mat[i][j] = m[k];
				k++;
			}
		}
		return res;
	}
	static OMat4 mat4_cast(const OVec4& q)

	{
		//converts quaternion to mat4
		OMat4 Result(1);

		float qxx(q.m_x * q.m_x);
		float qyy(q.m_y * q.m_y);
		float qzz(q.m_z * q.m_z);
		float qxz(q.m_x * q.m_z);
		float qxy(q.m_x * q.m_y);
		float qyz(q.m_y * q.m_z);
		float qwx(q.m_w * q.m_x);
		float qwy(q.m_w * q.m_y);
		float qwz(q.m_w * q.m_z);

		Result.m_mat[0][0] = float(1) - float(2) * (qyy + qzz);
		Result.m_mat[0][1] = float(2) * (qxy + qwz);
		Result.m_mat[0][2] = float(2) * (qxz - qwy);

		Result.m_mat[1][0] = float(2) * (qxy - qwz);
		Result.m_mat[1][1] = float(1) - float(2) * (qxx + qzz);
		Result.m_mat[1][2] = float(2) * (qyz + qwx);

		Result.m_mat[2][0] = float(2) * (qxz + qwy);
		Result.m_mat[2][1] = float(2) * (qyz - qwx);
		Result.m_mat[2][2] = float(1) - float(2) * (qxx + qyy);

		Result.m_mat[3][3] = 1.0f;

		return Result;
	}

	void setIdentity()
	{
		::memset(m_mat, 0, sizeof(float) * 16);
		m_mat[0][0] = 1;
		m_mat[1][1] = 1;
		m_mat[2][2] = 1;
		m_mat[3][3] = 1;
	}

	void removeTranslation()
	{
		m_mat[0][3] = 0;
		m_mat[1][3] = 0;
		m_mat[2][3] = 0;
		m_mat[3][3] = 1;
	}


	void setTranslation(const OVec3& translation)
	{
		m_mat[3][0] = translation.m_x;
		m_mat[3][1] = translation.m_y;
		m_mat[3][2] = translation.m_z;
	}

	void setScale(const OVec3& scale)
	{
		m_mat[0][0] = scale.m_x;
		m_mat[1][1] = scale.m_y;
		m_mat[2][2] = scale.m_z;
	}

	void setRotationX(float x)
	{
		m_mat[1][1] = cos(x);
		m_mat[1][2] = sin(x);
		m_mat[2][1] = -sin(x);
		m_mat[2][2] = cos(x);
	}

	void setRotationY(float y)
	{
		m_mat[0][0] = cos(y);
		m_mat[0][2] = -sin(y);
		m_mat[2][0] = sin(y);
		m_mat[2][2] = cos(y);
	}

	void setRotationZ(float z)
	{
		m_mat[0][0] = cos(z);
		m_mat[0][1] = sin(z);
		m_mat[1][0] = -sin(z);
		m_mat[1][1] = cos(z);
	}

	float getDeterminant()
	{
		OVec4 minor, v1, v2, v3;
		float det;

		v1 = OVec4(this->m_mat[0][0], this->m_mat[1][0], this->m_mat[2][0], this->m_mat[3][0]);
		v2 = OVec4(this->m_mat[0][1], this->m_mat[1][1], this->m_mat[2][1], this->m_mat[3][1]);
		v3 = OVec4(this->m_mat[0][2], this->m_mat[1][2], this->m_mat[2][2], this->m_mat[3][2]);


		minor.cross(v1, v2, v3);
		det = -(this->m_mat[0][3] * minor.m_x + this->m_mat[1][3] * minor.m_y + this->m_mat[2][3] * minor.m_z +
			this->m_mat[3][3] * minor.m_w);
		return det;
	}

	void inverse()
	{
		int a, i, j;
		OMat4 out;
		OVec4 v, vec[3];
		float det = 0.0f;

		det = this->getDeterminant();
		if (!det) return;
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (j != i)
				{
					a = j;
					if (j > i) a = a - 1;
					vec[a].m_x = (this->m_mat[j][0]);
					vec[a].m_y = (this->m_mat[j][1]);
					vec[a].m_z = (this->m_mat[j][2]);
					vec[a].m_w = (this->m_mat[j][3]);
				}
			}
			v.cross(vec[0], vec[1], vec[2]);

			out.m_mat[0][i] = pow(-1.0f, i) * v.m_x / det;
			out.m_mat[1][i] = pow(-1.0f, i) * v.m_y / det;
			out.m_mat[2][i] = pow(-1.0f, i) * v.m_z / det;
			out.m_mat[3][i] = pow(-1.0f, i) * v.m_w / det;
		}

		this->setMatrix(out);
	}


	void operator *=(const OMat4& matrix)
	{
		OMat4 out;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				out.m_mat[i][j] =
					m_mat[i][0] * matrix.m_mat[0][j] + m_mat[i][1] * matrix.m_mat[1][j] +
					m_mat[i][2] * matrix.m_mat[2][j] + m_mat[i][3] * matrix.m_mat[3][j];
			}
		}
		setMatrix(out);
	}

	OMat4 operator * (const OMat4& m2)
	{
		OMat4 res;
		res *= (*this);
		res *= (m2);
		return res;
	}

	OVec3 operator *(OVec3& in)
	{
		OVec3 out;

		out.m_x = in.m_x * m_mat[0][0] + in.m_y * m_mat[1][0] + in.m_z * m_mat[2][0] + m_mat[3][0];
		out.m_y = in.m_x * m_mat[0][1] + in.m_y * m_mat[1][1] + in.m_z * m_mat[2][1] + m_mat[3][1];
		out.m_z = in.m_x * m_mat[0][2] + in.m_y * m_mat[1][2] + in.m_z * m_mat[2][2] + m_mat[3][2];
		float w = in.m_x * m_mat[0][3] + in.m_y * m_mat[1][3] + in.m_z * m_mat[2][3] + m_mat[3][3];

		if (w != 0.0f)
		{
			out.m_x /= w;
			out.m_y /= w;
			out.m_z /= w;
		}
		return out;
	}

	void setMatrix(const OMat4& matrix)
	{
		::memcpy(m_mat, matrix.m_mat, sizeof(float) * 16);
	}
	void operator =(const OMat4& matrix)
	{
		::memcpy(m_mat, matrix.m_mat, sizeof(float) * 16);
	}

	OVec3 getZDirection()
	{
		return OVec3(m_mat[2][0], m_mat[2][1], m_mat[2][2]);
	}
	OVec3 getXDirection()
	{
		return OVec3(m_mat[0][0], m_mat[0][1], m_mat[0][2]);
	}
	OVec3 getTranslation()
	{
		return OVec3(m_mat[3][0], m_mat[3][1], m_mat[3][2]);
	}

	void setPerspectiveFovLH(float fov, float aspect, float znear, float zfar)
	{
		float h = 1.0f / tan(fov / 2.0f);
		float w = h / aspect;
		m_mat[0][0] = w;
		m_mat[1][1] = h;
		m_mat[2][2] = zfar / (zfar - znear);
		m_mat[2][3] = 1.0f;
		m_mat[3][2] = (-znear * zfar) / (zfar - znear);
		m_mat[3][3] = 0.0f;

	}

	void setOrthoLH(float width, float height, float near_plane, float far_plane)
	{
		setIdentity();
		m_mat[0][0] = 2.0f / width;
		m_mat[1][1] = 2.0f / height;
		m_mat[2][2] = 1.0f / (far_plane - near_plane);
		m_mat[3][2] = -(near_plane / (far_plane - near_plane));
	}
	void LookAtMatrix(const OVec3& cam_pos, const OVec3& target_pos, const OVec3& up)
	{

		OVec3 x = target_pos;//messy here confused::: right handed assumed
		x -= cam_pos;
		//x = x * -1;
		OVec3 cam_dir = OVec3::normalize(x);
		OVec3 right_axis = OVec3::normalize(OVec3::cross(up, cam_dir));
		OVec3 up_axis = OVec3::cross(cam_dir, right_axis);

		setIdentity();
		m_mat[0][0] = right_axis.m_x;
		m_mat[0][1] = right_axis.m_y;
		m_mat[0][2] = right_axis.m_z;

		m_mat[1][0] = up_axis.m_x;
		m_mat[1][1] = up_axis.m_y;
		m_mat[1][2] = up_axis.m_z;

		m_mat[2][0] = cam_dir.m_x;
		m_mat[2][1] = cam_dir.m_y;
		m_mat[2][2] = cam_dir.m_z;


		m_mat[3][0] = -OVec3::dot(right_axis, cam_pos);
		m_mat[3][1] = -OVec3::dot(up_axis, cam_pos);
		m_mat[3][2] = -OVec3::dot(cam_dir, cam_pos);

	}
	~OMat4()
	{
	}
public:
	float m_mat[4][4] = {};
};

static float radians(float degrees)
{
	return (degrees * 3.14) / 180;
}