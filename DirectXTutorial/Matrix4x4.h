#pragma once
#include <memory>
#include "Vector3D.h"
#include "Vector4D.h"

class Matrix4x4
{
public:
	void setValues(float* values16) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				m_mat[i][j] = values16[i*4 + j];
			}
		}
	}

	void setIdentity()
	{
		::memset(m_mat, 0, sizeof(float) * 16);
		m_mat[0][0] = 1;
		m_mat[1][1] = 1;
		m_mat[2][2] = 1;
		m_mat[3][3] = 1;
	}

	void setTranslation(const Vector3D& translation)
	{
		m_mat[3][0] = translation.x;
		m_mat[3][1] = translation.y;
		m_mat[3][2] = translation.z;
	}

	void setScale(const Vector3D& scale)
	{
		m_mat[0][0] = scale.x;
		m_mat[1][1] = scale.y;
		m_mat[2][2] = scale.z;
	}

	void setRotationX(const float x)
	{
		m_mat[1][1] = cos(x);
		m_mat[1][2] = sin(x);
		m_mat[2][1] = -sin(x);
		m_mat[2][2] = cos(x);
	}

	void setRotationY(const float y)
	{
		m_mat[0][0] = cos(y);
		m_mat[0][2] = -sin(y);
		m_mat[2][0] = sin(y);
		m_mat[2][2] = cos(y);
	}

	void setRotationZ(const float z)
	{
		m_mat[0][0] = cos(z);
		m_mat[0][1] = sin(z);
		m_mat[1][0] = -sin(z);
		m_mat[1][1] = cos(z);
	}

	float getDeterminant()
	{
		Vector4D minor, v1, v2, v3;
		float det;

		v1 = Vector4D(this->m_mat[0][0], this->m_mat[1][0], this->m_mat[2][0], this->m_mat[3][0]);
		v2 = Vector4D(this->m_mat[0][1], this->m_mat[1][1], this->m_mat[2][1], this->m_mat[3][1]);
		v3 = Vector4D(this->m_mat[0][2], this->m_mat[1][2], this->m_mat[2][2], this->m_mat[3][2]);


		minor.cross(v1, v2, v3);
		det = -(this->m_mat[0][3] * minor.x + this->m_mat[1][3] * minor.y + this->m_mat[2][3] * minor.z +
			this->m_mat[3][3] * minor.w);
		return det;
	}

	void inverse()
	{
		int a, i, j;
		Matrix4x4 out;
		Vector4D v, vec[3];
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
					vec[a].x = (this->m_mat[j][0]);
					vec[a].y = (this->m_mat[j][1]);
					vec[a].z = (this->m_mat[j][2]);
					vec[a].w = (this->m_mat[j][3]);
				}
			}
			v.cross(vec[0], vec[1], vec[2]);

			out.m_mat[0][i] = pow(-1.0f, i) * v.x / det;
			out.m_mat[1][i] = pow(-1.0f, i) * v.y / det;
			out.m_mat[2][i] = pow(-1.0f, i) * v.z / det;
			out.m_mat[3][i] = pow(-1.0f, i) * v.w / det;
		}

		this->setMatrix(out);
	}

	// Vector3D multiplication
	Vector3D operator*(const Vector3D& vec) const 
	{
		Vector4D vec4(vec.x, vec.y, vec.z, 1.0f);

		Vector4D out;
		out.x = m_mat[0][0] * vec4.x + m_mat[0][1] * vec4.y + m_mat[0][2] * vec4.z + m_mat[0][3] * vec4.w;
		out.y = m_mat[1][0] * vec4.x + m_mat[1][1] * vec4.y + m_mat[1][2] * vec4.z + m_mat[1][3] * vec4.w;
		out.z = m_mat[2][0] * vec4.x + m_mat[2][1] * vec4.y + m_mat[2][2] * vec4.z + m_mat[2][3] * vec4.w;
		out.w = m_mat[3][0] * vec4.x + m_mat[3][1] * vec4.y + m_mat[3][2] * vec4.z + m_mat[3][3] * vec4.w;

		return Vector3D(out.x, out.y, out.z);
	}

	// Vector4D multiplication
	Vector4D operator *(const Vector4D& vector) const
	{
		Vector4D result;
		result.x = m_mat[0][0] * vector.x + m_mat[0][1] * vector.y + m_mat[0][2] * vector.z + m_mat[0][3] * vector.w;
		result.y = m_mat[1][0] * vector.x + m_mat[1][1] * vector.y + m_mat[1][2] * vector.z + m_mat[1][3] * vector.w;
		result.z = m_mat[2][0] * vector.x + m_mat[2][1] * vector.y + m_mat[2][2] * vector.z + m_mat[2][3] * vector.w;
		result.w = m_mat[3][0] * vector.x + m_mat[3][1] * vector.y + m_mat[3][2] * vector.z + m_mat[3][3] * vector.w;
		return result;
	}

	void operator *=(const Matrix4x4& matrix)
	{
		Matrix4x4 out;
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

	void setMatrix(const Matrix4x4& matrix)
	{
		::memcpy(m_mat, matrix.m_mat, sizeof(float) * 16);
	}

	void setOrthoLH(const float width, const float height, const float near_plane, const float far_plane)
	{
		setIdentity();
		m_mat[0][0] = 2.0f / width;
		m_mat[1][1] = 2.0f / height;
		m_mat[2][2] = 1.0f / (far_plane - near_plane);
		m_mat[3][2] = -(near_plane / (far_plane - near_plane));
	}

	void setPerspectiveFovLH(const float fov, const float aspect, const float znear, const float zfar)
	{
		float yscale = 1.0f / tan(fov / 2.0f);
		float xscale = yscale / aspect;
		m_mat[0][0] = xscale;
		m_mat[1][1] = yscale;
		m_mat[2][2] = zfar / (zfar - znear);
		m_mat[2][3] = 1.0f;
		m_mat[3][2] = (-znear * zfar) / (zfar - znear);
	}

	const Vector3D getZDirection() const
	{
		return Vector3D(m_mat[2][0], m_mat[2][1], m_mat[2][2]);
	}
	const Vector3D getYDirection() const
	{
		return Vector3D(m_mat[1][0], m_mat[1][1], m_mat[1][2]);
	}
	const Vector3D getXDirection() const
	{
		return Vector3D(m_mat[0][0], m_mat[0][1], m_mat[0][2]);
	}
	const Vector3D getTranslation() const
	{
		return Vector3D(m_mat[3][0], m_mat[3][1], m_mat[3][2]);
	}

	const Vector3D getEulerAngles() const
	{
		Vector3D euler;

		// Yaw (Z-axis rotation)
		euler.z = -atan2(m_mat[1][0], m_mat[0][0]);

		// Pitch (Y-axis rotation)
		euler.y = asin(m_mat[2][0]);

		// Roll (X-axis rotation)
		euler.x = -atan2(m_mat[2][1], m_mat[2][2]);

		return euler;
	}

	const void PrintMatrix() const {
		std::cout << "[" << m_mat[0][0] << ", " << m_mat[0][1] << ", " << m_mat[0][2] << ", " << m_mat[0][3] << ",\n";
		std::cout << " " << m_mat[1][0] << ", " << m_mat[1][1] << ", " << m_mat[1][2] << ", " << m_mat[1][3] << ",\n";
		std::cout << " " << m_mat[2][0] << ", " << m_mat[2][1] << ", " << m_mat[2][2] << ", " << m_mat[2][3] << ",\n";
		std::cout << " " << m_mat[3][0] << ", " << m_mat[3][1] << ", " << m_mat[3][2] << ", " << m_mat[3][3] << "]\n";
	}

	float* Flatten() const {
		float output[16];
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				output[i * 4 + j] = m_mat[i][j];
			}
		}
		return output;
	}

public:
	float m_mat[4][4] = {};
};