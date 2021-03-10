#pragma once

#include <math.h>
#include <stdio.h>
#include <iostream>
#include <functional>

namespace m3d {

	const float Pi = 3.1415926535;

	float deg_to_rad(float angle_in_degrees)
	{
		return angle_in_degrees / 180 * Pi;
	}

	float rad_to_deg(float angle_in_radians)
	{
		return angle_in_radians * 180 / Pi;
	}

	/* Note
	With OpenGL and GLSL Matricies are column major ordered >> [x][y]
	*/

	/* Note
	With OpenGL and GLSL Matricies are column major ordered >> [x][y]
	*/

	class vec2 {
		public:
		float y, x = 0; 	// declared in reverse because stack pointer go in reverse
		float* array = &x;	// point array to start of memory

		// Constructor
		vec2(float f=0) {
			this->x = f;
			this->y = f;
		}
		vec2(float x=0, float y=0) {

			this->x = x;
			this->y = y;
			
		}

		// Static Functions
		static float Length(vec2 v) {
			return sqrtf(v.x * v.x + v.y * v.y);
		}
		static vec2 Absolute(vec2 v)
		{
			return vec2(
					v.x < 0 ? v.x * -1 : v.x,
					v.y < 0 ? v.y * -1 : v.y
				);
		}
		static vec2 Normalise(vec2 v) {
			float len = vec2::Length(v);
			if (len > 0) {
				return vec2(v.x / len, v.y / len);
			} else {
				return vec2(0, 0);
			}
		}
		static float Dot(vec2 a, vec2 b) {
			return a.x * b.x + a.y * b.y;
		}
		static vec2 Project(vec2 v, vec2 onto) {
			return onto * (Dot(v, onto) / Dot(onto, onto));
		}
		static float AngleBetween(vec2 a, vec2 b) {
			return acosf(Dot(a, b) / (vec2::Length(a) * vec2::Length(b)));
		}
		
		// debug
		static void Print(vec2 v) {
			std::cout << "x: " << v.x << " y: " << v.y << std::endl;
		}


		// --- OPERATOR OVERLOADS --- //

		// Addition
		vec2 operator+(const vec2& v) {
			return vec2(
				this->x + v.x,
				this->y + v.y
			);
		}
		vec2 operator+(const float& f) {
			return vec2(
				this->x + f,
				this->y + f
			);
		}
		// Subtraction
		vec2 operator-(const vec2& v) {
			return vec2(
				this->x - v.x,
				this->y - v.y
			);
		}
		vec2 operator-(const float& f) {
			return vec2(
				this->x - f,
				this->y - f
			);
		}
		// Multiplication
		vec2 operator*(const vec2& v) {
			return vec2(
				this->x * v.x,
				this->y * v.y
			);
		}
		vec2 operator*(const float& f) {
			return vec2(
				this->x * f,
				this->y * f
			);
		}
		// Division
		vec2 operator/(const vec2& v) {
			float vx = v.x != 0 ? v.x : 1;
			float vy = v.y != 0 ? v.y : 1;
			return vec2(
				this->x / vx,
				this->y / vy
			);
		}
		vec2 operator/(const float& f) {
			float fd = f != 0 ? f : 1;
			return vec2(
				this->x / fd,
				this->y / fd
			);
		}
	};

	class vec3 {
		public:
		float z, y, x =0;
		float* array = &x;

		vec3(float f){
			this->x = f;
			this->y = f;
			this->z = f;
		}
		vec3(vec2 v, float f=0) {
			this->x = v.x;
			this->y = v.y;
			this->z = f;
		}
		vec3(float x=0, float y=0, float z=0) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		/*
		float Length()
		{
			return this->x * v.x + this->y * v.y + this->z *v.z;
		}
		vec3 Normalise()
		{
			return acosf(this->Dot(v) / (this->Length() * v.Length()));
		}
		vec3 Absolute()
		{
			return vec3(
				this->x < 0 ? this->x * -1 : this->x,
				this->y < 0 ? this->y * -1 : this->y,
				this->z < 0 ? this->z * -1 : this->z
			);
		}
		*/
		
		// static functions
		static float Length(vec3 v)
		{
			return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
		}
		static vec3 Normalise(vec3 v)
		{
			float len = Length(v);
			if (len > 0)
				return vec3 (v.x / len, v.y / len, v.z / len);
			else
				return vec3 (0, 0, 0);
		}
		static vec3 Absolute(vec3 v)
		{
			return vec3(
					v.x < 0 ? v.x * -1 : v.x,
					v.y < 0 ? v.y * -1 : v.y,
					v.z < 0 ? v.z * -1 : v.z
				);
		}
		static float Dot(vec3 a, vec3 b)
		{
			return a.x * b.x + a.y * b.y + a.z *b.z;
		}
		static vec3 Project(vec3 v, vec3 onto)
		{
			return onto * (Dot(v, onto) / Dot(onto, onto));
		}
		static vec3 Cross(vec3 a, vec3 b)
		{
			return vec3
			{
				a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x
			};
		}
		static float Angle_Between(vec3 a, vec3 b)
		{
			return acosf(Dot(a, b) / (Length(a)* Length(b)));
		}

		// debug
		static void Print(vec3 v) {
			std::cout << "x: " << v.x << " y: " << v.y << " z: " << v.z << std::endl;
		}


		// --- OPERATOR OVERLOADS --- //
		// Addition
		vec3 operator+(const vec3& v) {
			return vec3(
				this->x + v.x,
				this->y + v.y,
				this->z + v.z
			);
		}
		vec3 operator+(const float& f) {
			return vec3(
				this->x + f,
				this->y + f,
				this->z + f
			);
		}
		// Subtraction
		vec3 operator-(const vec3& v) {
			return vec3(
				this->x - v.x,
				this->y - v.y,
				this->z - v.z
			);
		}
		vec3 operator-(const float& f) {
			return vec3(
				this->x - f,
				this->y - f,
				this->z - f
			);
		}
		// Multiplication
		vec3 operator*(const vec3& v) {
			return vec3(
				this->x * v.x,
				this->y * v.y,
				this->z * v.z
			);
		}
		vec3 operator*(const float& f) {
			return vec3(
				this->x * f,
				this->y * f,
				this->z * f
			);
		}
		// Division
		vec3 operator/(const vec3& v) {
			float vx = v.x != 0 ? v.x : 1;
			float vy = v.y != 0 ? v.y : 1;
			float vz = v.z != 0 ? v.z : 1;
			return vec3(
				this->x / vx,
				this->y / vy,
				this->z / vz
			);
		}
		vec3 operator/(const float& f) {
			float fd = f != 0 ? f : 1;
			return vec3(
				this->x / fd,
				this->y / fd,
				this->z / fd
			);
		}
	};

	/* 2 x 2 Matrix */
	float m2_Determinant(float m00, float m01, float m10, float m11)
	{
		return m00 * m11 - m01 * m10;
	}


	/* 3 x 3 Matrix */
	class mat3 {
		public:
		float m22, m12, m02;
		float m21, m11, m01;
		float m20, m10, m00; 
		float* array = &m00;

		// Constructors
		mat3(	float m00=1, float m01=0, float m02=0,
				float m10=0, float m11=1, float m12=0,
				float m20=0, float m21=0, float m22=1) {
			this->m00 = m00; this->m01 = m01; this->m02 = m02;
			this->m10 = m10; this->m11 = m11; this->m12 = m12;
			this->m20 = m20; this->m21 = m21; this->m22 = m22;
		}
		mat3(vec3 v1, vec3 v2, vec3 v3) {
			this->m00 = v1.x; this->m01 = v2.x; this->m02 = v3.x;
			this->m10 = v1.y; this->m11 = v2.y; this->m12 = v3.y;
			this->m20 = v1.z; this->m21 = v2.z; this->m22 = v3.z;
		}
		mat3(float f) {
			this->m00 = f; this->m01 = f; this->m02 = f;
			this->m10 = f; this->m11 = f; this->m12 = f;
			this->m20 = f; this->m21 = f; this->m22 = f;
		}

		// Selector
		float m(int col, int row) {
			return this->array[col * 3 + row];
		}
		void m(int col, int row, float f) {
			this->array[col * 3 + row] = f;
		}

		// static functions
		static mat3 Identity() {
			return mat3();
		}
		static mat3 Transpose(mat3 m) {
			return mat3
			(
				m.m00, m.m10, m.m20,
				m.m01, m.m11, m.m21,
				m.m02, m.m12, m.m22
			);
		}
		static mat3 MatrixMinors(mat3 a) {
			mat3 m;
			m.m00 = m2_Determinant(a.m11, a.m12, a.m21, a.m22);
			m.m01 = m2_Determinant(a.m10, a.m12, a.m20, a.m22);	
			m.m02 = m2_Determinant(a.m10, a.m11, a.m20, a.m21);

			m.m10 = m2_Determinant(a.m01, a.m21, a.m02, a.m22);
			m.m11 = m2_Determinant(a.m00, a.m02, a.m20, a.m22);
			m.m12 = m2_Determinant(a.m00, a.m01, a.m20, a.m21);

			m.m20 = m2_Determinant(a.m01, a.m02, a.m11, a.m12);
			m.m21 = m2_Determinant(a.m00, a.m02, a.m10, a.m12);
			m.m22 = m2_Determinant(a.m00, a.m01, a.m10, a.m11);
			return m;
		}
		static mat3 MatrixCoFactors(mat3 m) {
			m = MatrixMinors(m);
			m.m01 = m.m01 * -1;
			m.m10 = m.m10 * -1;
			m.m12 = m.m12 * -1;
			m.m21 = m.m21 * -1;
			return m;
		}
		static float Determinant(mat3 a) {
			float f0 = m2_Determinant(a.m11, a.m12, a.m21, a.m22);
			float f1 = m2_Determinant(a.m10, a.m12, a.m20, a.m22);
			float f2 = m2_Determinant(a.m10, a.m11, a.m20, a.m21);

			return a.m00 * f0 - a.m01 * f1 + a.m02 * f2;
		}
		static mat3 Inverse(mat3 a) {
			return MatrixCoFactors(a) * (1 / Determinant(a));
		}

	/*
		mat3 Transpose() {
			return mat3
			(
				this->m00, this->m10, this->m20,
				this->m01, this->m11, this->m21,
				this->m02, this->m12, this->m22
			);
		}

		mat3 MatrixMinors()
		{
			mat3 m;
			m.m00 = m2_Determinant(this->m11, this->m12, this->m21, this->m22);
			m.m01 = m2_Determinant(this->m10, this->m12, this->m20, this->m22);	
			m.m02 = m2_Determinant(this->m10, this->m11, this->m20, this->m21);

			m.m10 = m2_Determinant(this->m01, this->m21, this->m02, this->m22);
			m.m11 = m2_Determinant(this->m00, this->m02, this->m20, this->m22);
			m.m12 = m2_Determinant(this->m00, this->m01, this->m20, this->m21);

			m.m20 = m2_Determinant(this->m01, this->m02, this->m11, this->m12);
			m.m21 = m2_Determinant(this->m00, this->m02, this->m10, this->m12);
			m.m22 = m2_Determinant(this->m00, this->m01, this->m10, this->m11);
			return m;
		}

		mat3 MatrixCoFactors()
		{
			mat3 m = this->MatrixMinors();
			m.m01 = m.m01 * -1;
			m.m10 = m.m10 * -1;
			m.m12 = m.m12 * -1;
			m.m21 = m.m21 * -1;
			return m;
		}

		float Determinant()
		{
			float f0 = m2_Determinant(this->m11, this->m12, this->m21, this->m22);
			float f1 = m2_Determinant(this->m10, this->m12, this->m20, this->m22);
			float f2 = m2_Determinant(this->m10, this->m11, this->m20, this->m21);
			return this->m00 * f0 - this->m01 * f1 + this->m02 * f2;
		}

		mat3 Inverse()
		{
			if (this->Determinant() == 0) {
				return mat3(
					this->m00, this->m01, this->m02,
					this->m10, this->m11, this->m12,
					this->m20, this->m21, this->m22 
				);
			} else {
				return this->MatrixCoFactors() * (1 / this->Determinant());
			}
		}
	*/

		/// Operator Overloads ///
		// Addition
		mat3 operator+(const mat3& m) {
			return mat3(
				this->m00 + m.m00, this->m01 + m.m01, this->m02 + m.m02,
				this->m10 + m.m10, this->m11 + m.m11, this->m12 + m.m12,
				this->m20 + m.m20, this->m21 + m.m21, this->m22 + m.m22
			);
		}
		mat3 operator+(const float& f) {
			return mat3(
				this->m00 + f, this->m01 + f, this->m02 + f,
				this->m10 + f, this->m11 + f, this->m12 + f,
				this->m20 + f, this->m21 + f, this->m22 + f
			);
		}
		// Subtraction
		mat3 operator-(const mat3& m) {
			return mat3(
				this->m00 - m.m00, this->m01 - m.m01, this->m02 - m.m02,
				this->m10 - m.m10, this->m11 - m.m11, this->m12 - m.m12,
				this->m20 - m.m20, this->m21 - m.m21, this->m22 - m.m22
			);
		}
		mat3 operator-(const float& f) {
			return mat3(
				this->m00 - f, this->m01 - f, this->m02 - f,
				this->m10 - f, this->m11 - f, this->m12 - f,
				this->m20 - f, this->m21 - f, this->m22 - f
			);
		}
		// Multiplication
		mat3 operator*(const mat3& m) {
			mat3 n = mat3();
			for (int i = 0; i < 3; i++) {
				for (int j =0; j < 3; j++) {
					for (int k = 0; k < 3; k++) {
						n.array[j * 3 + i] += this->array[j * 3 + k] * m.array[k * 3 + i];
					}	
				}
			}
			return n;
		}
		vec3 operator*(const vec3& v) {
			vec3 w = vec3();
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					w.array[j] += this->array[j * 3 + k] * v.array[k];
				}	
			}
			return w;
		}
		mat3 operator*(const float& f) {
			return mat3(
				this->m00 * f, this->m01 * f, this->m02 * f,
				this->m10 * f, this->m11 * f, this->m12 * f,
				this->m20 * f, this->m21 * f, this->m22 * f
			);
		}
	};

	
	/*
	Column Major vs Row Major only affects the way in which arrays are stored

	COLUMN MAJORED
	// | m[0][0]  m[1][0]  m[2][0]  m[3][0] |
	// | m[0][1]  m[1][1]  m[2][1]  m[3][1] |
	// | m[0][2]  m[1][2]  m[2][2]  m[3][2] |
	// | m[0][3]  m[1][3]  m[2][3]  m[3][3] |

	ROW MAJORED
	// | m[0][0]  m[0][1]  m[0][2]  m[0][3] |
	// | m[1][0]  m[1][1]  m[1][2]  m[1][3] |
	// | m[2][0]  m[2][1]  m[2][2]  m[2][3] |
	// | m[3][0]  m[3][1]  m[3][2]  m[3][3] |

	The data is still always represented as:

	a.00, b.01, c.02, d.03,
	e.10, f.11, g.12, h.13,
	i.20, j.21, k.22, l.23,
	m.30, n.31, o.32, p.33,

	*/

	/* 4 x 4 Matrix */
	class mat4 {
		public:
		float m33, m23, m13, m03;
		float m32, m22, m12, m02;
		float m31, m21, m11, m01;
		float m30, m20, m10, m00; 
		float* array = &m00;

		// Constructors
		mat4(	float m00=1, float m01=0, float m02=0, float m03=0,
				float m10=0, float m11=1, float m12=0, float m13=0,
				float m20=0, float m21=0, float m22=1, float m23=0,
				float m30=0, float m31=0, float m32=0, float m33=1) {
			this->m00 = m00; this->m01 = m01; this->m02 = m02; this->m03 = m03;
			this->m10 = m10; this->m11 = m11; this->m12 = m12; this->m13 = m13;
			this->m20 = m20; this->m21 = m21; this->m22 = m22; this->m23 = m23;
			this->m30 = m30; this->m31 = m31; this->m32 = m32; this->m33 = m33;
		}
		mat4(float f) {
			this->m00 = f; this->m01 = f; this->m02 = f; this->m03 = f;
			this->m10 = f; this->m11 = f; this->m12 = f; this->m13 = f;
			this->m20 = f; this->m21 = f; this->m22 = f; this->m23 = f;
			this->m30 = f; this->m31 = f; this->m32 = f; this->m33 = f;
		}

		// Selector
		float m(int col, int row) {
			return this->array[col * 4 + row];
		}
		void m(int col, int row, float f) {
			this->array[col * 4 + row] = f;
		}

		// Static functions
		static mat4 Identity() {
			return mat4();
		}
		static mat4 Invert_Affine(mat4 m) {
			// Invert 3x3 part of the 4x4 matric that contains the rotation ( R )

				// Calculate CoFactor Matrix of R
			float c00 = m.m11 * m.m22 - m.m12 * m.m21, c10 = -(m.m01*m.m22 - m.m02 * m.m21), c20 = m.m01 * m.m12 - m.m02 * m.m11;
			float c01 = -(m.m10*m.m22 - m.m12 * m.m20), c11 = m.m00 * m.m22 - m.m02 * m.m20, c21 = -(m.m00*m.m12 - m.m02 * m.m10);
			float c02 = m.m10 * m.m21 - m.m11 * m.m20, c12 = -(m.m00*m.m21 - m.m01 * m.m20), c22 = m.m00 * m.m11 - m.m01 * m.m10;

			// Calculate determinant by using the calculated cofactor maxtrix
			// Second sign is already minus from the covfactor matrix
			float det = m.m00 * c00 + m.m10 * c10 + m.m20 * c20;
			if (fabsf(det == 0))
			{
				return mat4::Identity();
			}

			// Calculate inverse of R by dividing the transposed co factor matrix by the determinant
			float i00 = c00 / det, i10 = c01 / det, i20 = c02 / det;
			float i01 = c10 / det, i11 = c11 / det, i21 = c12 / det;
			float i02 = c20 / det, i12 = c21 / det, i22 = c22 / det;

			// Combine the inverted R with the inverted translation
			return mat4
			(
				i00, i10, i20, -(i00 * m.m30 + i10 * m.m31 + i20 * m.m32),
				i01, i11, i21, -(i01 * m.m30 + i11 * m.m31 + i21 * m.m32),
				i02, i12, i22, -(i02 * m.m30 + i12 * m.m31 + i22 * m.m32),
				0, 0, 0, 1
			);
		}
		static vec3 Mul_Pos(mat4 m, vec3 pos)
		{
			vec3 result = vec3
			(
				m.m00 * pos.x + m.m10 * pos.y + m.m20 * pos.z + m.m30,
				m.m01 * pos.x + m.m11 * pos.y + m.m21 * pos.z + m.m31,
				m.m02 * pos.x + m.m12 * pos.y + m.m22 * pos.z + m.m32
			);

			float w = m.m03 * pos.x + m.m13 * pos.y + m.m23 * pos.z + m.m33;
			if (w != 0 && w != 1)
			{
				return vec3(result.x / w, result.y / w, result.z / w);
			}

			return result;
		}
		static mat4 Transpose(mat4 m) {
			return mat4
			(
				m.m00, m.m10, m.m20, m.m30,
				m.m01, m.m11, m.m21, m.m31,
				m.m02, m.m12, m.m22, m.m32,
				m.m03, m.m13, m.m23, m.m33
			);
		}
		
		/*
		mat4 m4_Transpose(mat4 m)
	{
		return mat4
		(
			m.m00, m.m10, m.m20, m.m30,
			m.m01, m.m11, m.m21, m.m31,
			m.m02, m.m12, m.m22, m.m32,
			m.m03, m.m13, m.m23, m.m33
		);
	}

		mat4 m4_Rotation(float angle_in_rad, vec3 axis)
		{
			vec3 normalised_axis = vec3::Normalise(axis);
			float x = normalised_axis.x;
			float y = normalised_axis.y;
			float z = normalised_axis.z;
			float s = sinf(angle_in_rad);
			float c = cosf(angle_in_rad);

			return mat4
			(
				c + x * x*(1 - c), x*y*(1 - c) - z * s, x*z*(1 - c) + y * s, 0,
				y*x*(1 - c) + z * s, c + y * y*(1 - c), y*z*(1 - c) - x * s, 0,
				z*x*(1 - c) - y * s, z*y*(1 - c) + x * s, c + z * z*(1 - c), 0,
				0, 0, 0, 1
			);
		}

		mat4 m4_Orthographic(float left, float right, float bottom, float top, float zNear, float zFar)
		{
			float tx = -(right + left) / (right - left);
			float ty = -(top + bottom) / (top - bottom);
			float tz = -(zFar + zNear) / (zFar - zNear);
			return mat4
			(
				2 / (right - left), 0, 0, tx,
				0, 2 / (top - bottom), 0, ty,
				0, 0, 2 / (zFar - zNear), tz,
				0, 0, 0, 1
			);
		}

		mat4 m4_Perspective(float vertical_FOV_in_deg, float aspect_ratio, float near_view_distance, float far_view_distance)
		{
			float FOVy_in_rad = vertical_FOV_in_deg * Pi / 180;
			float f = 1.0f / tanf(FOVy_in_rad / 2.0f);
			float nvd = near_view_distance;
			float fvd = far_view_distance;
			return mat4
			(
				f / aspect_ratio, 0, 0, 0,
				0, f, 0, 0,
				0, 0, (fvd + nvd) / (nvd - fvd), (2 * fvd * nvd) / (nvd - fvd),
				0, 0, -1, 0
			);
		}
		
		mat4 m4_Look_At(vec3 from, vec3 to, vec3 up)
		{
			vec3 z = vec3::Normalise(to - from) * -1;
			vec3 x = vec3::Normalise(vec3::Cross(up, z));
			vec3 y = vec3::Cross(z, x);
			return mat4
			(
				x.x, x.y, x.z, -vec3::Dot(from, x),
				y.x, y.y, y.z, -vec3::Dot(from, y),
				z.x, z.y, z.z, -vec3::Dot(from, z),
				0, 0, 0, 1
			);
		}

		mat4 m4_Invert_Affine(mat4 m)
		{
			// Invert 3x3 part of the 4x4 matric that contains the rotation ( R )

				// Calculate CoFactor Matrix of R
			float c00 = m.m11 * m.m22 - m.m12 * m.m21, c10 = -(m.m01*m.m22 - m.m02 * m.m21), c20 = m.m01 * m.m12 - m.m02 * m.m11;
			float c01 = -(m.m10*m.m22 - m.m12 * m.m20), c11 = m.m00 * m.m22 - m.m02 * m.m20, c21 = -(m.m00*m.m12 - m.m02 * m.m10);
			float c02 = m.m10 * m.m21 - m.m11 * m.m20, c12 = -(m.m00*m.m21 - m.m01 * m.m20), c22 = m.m00 * m.m11 - m.m01 * m.m10;

			// Calculate determinant by using the calculated cofactor maxtrix
			// Second sign is already minus from the covfactor matrix
			float det = m.m00 * c00 + m.m10 * c10 + m.m20 * c20;
			if (fabsf(det == 0))
			{
				return mat4::Identity();
			}

			// Calculate inverse of R by dividing the transposed co factor matrix by the determinant
			float i00 = c00 / det, i10 = c01 / det, i20 = c02 / det;
			float i01 = c10 / det, i11 = c11 / det, i21 = c12 / det;
			float i02 = c20 / det, i12 = c21 / det, i22 = c22 / det;

			// Combine the inverted R with the inverted translation
			return mat4
			(
				i00, i10, i20, -(i00*m.m30 + i10 * m.m31 + i20 * m.m32),
				i01, i11, i21, -(i01*m.m30 + i11 * m.m31 + i21 * m.m32),
				i02, i12, i22, -(i02*m.m30 + i12 * m.m31 + i22 * m.m32),
				0, 0, 0, 1
			);
		}
		*/

		// debug
		void Print() {
			for (int j = 0; j < 4; j++)
			{
				for (int i = 0; i < 4; i++)
				{
					printf("%f, ", this->array[j * 4 + i]);
				}
				printf("\n");
			}
		}

		// --- Operator Overload --- //
		// Addition
		mat4 operator+(const mat4& m) {
			return mat4(
				this->m00 + m.m00, this->m01 + m.m01, this->m02 + m.m02, this->m03 + m.m03,
				this->m10 + m.m10, this->m11 + m.m11, this->m12 + m.m12, this->m13 + m.m13,
				this->m20 + m.m20, this->m21 + m.m21, this->m22 + m.m22, this->m23 + m.m23,
				this->m30 + m.m30, this->m31 + m.m31, this->m32 + m.m32, this->m33 + m.m33
			);
		}
		mat4 operator+(const float& f) {
			return mat4(
				this->m00 + f, this->m01 + f, this->m02 + f, this->m03 + f,
				this->m10 + f, this->m11 + f, this->m12 + f, this->m13 + f,
				this->m20 + f, this->m21 + f, this->m22 + f, this->m23 + f,
				this->m30 + f, this->m31 + f, this->m32 + f, this->m33 + f
			);
		}
		// Subtraction
		mat4 operator-(const mat4& m) {
			return mat4(
				this->m00 - m.m00, this->m01 - m.m01, this->m02 - m.m02, this->m03 - m.m03,
				this->m10 - m.m10, this->m11 - m.m11, this->m12 - m.m12, this->m13 - m.m13,
				this->m20 - m.m20, this->m21 - m.m21, this->m22 - m.m22, this->m23 - m.m23,
				this->m30 - m.m30, this->m31 - m.m31, this->m32 - m.m32, this->m33 - m.m33
			);
		}
		mat4 operator-(const float& f) {
			return mat4(
				this->m00 - f, this->m01 - f, this->m02 - f, this->m03 - f,
				this->m10 - f, this->m11 - f, this->m12 - f, this->m13 - f,
				this->m20 - f, this->m21 - f, this->m22 - f, this->m23 - f,
				this->m30 - f, this->m31 - f, this->m32 - f, this->m33 - f
			);
		}
		// Multiplication
		mat4 operator*(const mat4& m) {
			mat4 n = mat4();
			for (int i = 0; i < 4; i++) {
				for (int j =0; j < 4; j++) {
					for (int k = 0; k < 4; k++) {
						n.array[j * 4 + i] += this->array[j * 4 + k] * m.array[k * 4 + i];
					}	
				}
			}
			return n;
		}
		mat4 operator*(const float& f) {
			return mat4(
				this->m00 * f, this->m01 * f, this->m02 * f,
				this->m10 * f, this->m11 * f, this->m12 * f,
				this->m20 * f, this->m21 * f, this->m22 * f
			);
		}
	};

	/// --- Transformations --- ///
	mat4 Translation(vec3 offset) {
		return mat4(
			1, 0, 0, offset.x,
			0, 1, 0, offset.y,
			0, 0, 1, offset.z,
			0, 0, 0, 1
		);
	}
	mat4 Translation(float x=0, float y=0, float z=0) {
		return mat4(
			1, 0, 0, x,
			0, 1, 0, y,
			0, 0, 1, z,
			0, 0, 0, 1
		);
	}
	mat4 Scaling(vec3 s) {
		return mat4(
			s.x, 	0, 		0, 		0,
			0, 		s.y, 	0, 		0,
			0, 		0, 		s.z, 	0,
			0, 		0, 		0, 		1
		);
	}
	mat4 Scaling(float x=0, float y=0, float z=0) {
		return mat4(
			x, 	0, 	0, 	0,
			0, 	y, 	0, 	0,
			0, 	0, 	z, 	0,
			0, 	0, 	0, 	1
		);
	}
	mat4 RotationX(float angle_in_rad) {
		float s = sinf(angle_in_rad);
		float c = cosf(angle_in_rad);
		return mat4
		(
			1, 0, 0, 0,
			0, c, -s, 0,
			0, s, c, 0,
			0, 0, 0, 1
		);
	}
	mat4 RoationY(float angle_in_rad) {
		float s = sinf(angle_in_rad);
		float c = cosf(angle_in_rad);
		return mat4
		(
			c, 0, s, 0,
			0, 1, 0, 0,
			-s, 0, c, 0,
			0, 0, 0, 1
		);

	}
	mat4 RotationZ(float angle_in_rad) {
		float s = sinf(angle_in_rad);
		float c = cosf(angle_in_rad);
		return mat4
		(
			c, -s, 0, 0,
			s, c, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	}
	mat4 Rotation(float angle_in_rad, vec3 axis)
	{
		vec3 normalised_axis = vec3::Normalise(axis);
		float x = normalised_axis.x;
		float y = normalised_axis.y;
		float z = normalised_axis.z;
		float s = sinf(angle_in_rad);
		float c = cosf(angle_in_rad);

		return mat4
		(
			c + x * x*(1 - c), x*y*(1 - c) - z * s, x*z*(1 - c) + y * s, 0,
			y*x*(1 - c) + z * s, c + y * y*(1 - c), y*z*(1 - c) - x * s, 0,
			z*x*(1 - c) - y * s, z*y*(1 - c) + x * s, c + z * z*(1 - c), 0,
			0, 0, 0, 1
		);
	}
	mat4 Orthographic(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		float tx = -(right + left) / (right - left);
		float ty = -(top + bottom) / (top - bottom);
		float tz = -(zFar + zNear) / (zFar - zNear);
		return mat4
		(
			2 / (right - left), 0, 0, tx,
			0, 2 / (top - bottom), 0, ty,
			0, 0, 2 / (zFar - zNear), tz,
			0, 0, 0, 1
		);
	}
	mat4 Perspective(float vertical_FOV_in_deg, float aspect_ratio, float near_view_distance, float far_view_distance)
	{
		float FOVy_in_rad = vertical_FOV_in_deg * Pi / 180;
		float f = 1.0f / tanf(FOVy_in_rad / 2.0f);
		float nvd = near_view_distance;
		float fvd = far_view_distance;
		return mat4
		(
			f / aspect_ratio, 0, 0, 0,
			0, f, 0, 0,
			0, 0, (fvd + nvd) / (nvd - fvd), (2 * fvd * nvd) / (nvd - fvd),
			0, 0, -1, 0
		);
	}
	mat4 Look_At(vec3 from, vec3 to, vec3 up)
	{
		vec3 z = vec3::Normalise(to - from) * -1;
		vec3 x = vec3::Normalise(vec3::Cross(up, z));
		vec3 y = vec3::Cross(z, x);
		return mat4
		(
			x.x, x.y, x.z, -vec3::Dot(from, x),
			y.x, y.y, y.z, -vec3::Dot(from, y),
			z.x, z.y, z.z, -vec3::Dot(from, z),
			0, 0, 0, 1
		);
	}
}