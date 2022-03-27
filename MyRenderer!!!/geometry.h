// ***********��ѧ��***********
#include <iostream>
#include <vector>
#include <initializer_list>

#pragma once

// PI
#define PI 3.1415926

// *******************************************************************************
// ����
// *******************************************************************************

extern class Vec2f;
extern class Vec3f;
extern class Vec4f;

class Vec2i 
{
public:
	int x;
	int y;

	inline Vec2i() {}

	inline Vec2i(int a, int b) {
		this->x = a;
		this->y = b;
	}

	int& operator[] (int n) {
		if (n == 0) {
			return x;
		}
		else if (n == 1) {
			return y;
		}
		else {
			std::cout << "Vec2i��ȡ����x,y����ķ��������ﷵ��x������\n" << std::endl;
			return x;
		}
	}

	Vec2i operator+ (Vec2i v0) {
		Vec2i v;
		v.x = this->x + v0.x;
		v.y = this->y + v0.y;

		return v;
	}

	Vec2i operator- (Vec2i v0) {
		Vec2i v;
		v.x = this->x - v0.x;
		v.y = this->y - v0.y;

		return v;
	}

	Vec2i operator* (float alpha) {
		Vec2i v;
		v.x = this->x * alpha;
		v.y = this->y * alpha;

		return v;
	}
};

class Vec2f
{
public:
	union {
		struct {
			float x;
			float y;
		};
		float val[2];
	};

	inline Vec2f() {}
	inline Vec2f(float a, float b) {
		this->x = a;
		this->y = b;
	}


	float& operator[] (int n) {
		return val[n];
	}

	Vec2f operator+ (Vec2f v0) {
		Vec2f v;
		v.x = this->x + v0.x;
		v.y = this->y + v0.y;

		return v;
	}

	Vec2f operator- (Vec2f v0) {
		Vec2f v;
		v.x = this->x - v0.x;
		v.y = this->y - v0.y;

		return v;
	}

	Vec2f operator* (float alpha) {
		Vec2f v;
		v.x = this->x * alpha;
		v.y = this->y * alpha;

		return v;
	}

	// ����ά�������䵽��ά or ��ά
	Vec3f embed_3();
	Vec4f embed_4();

	// ��ӡ����
	void printfVec() {
		std::cout << "x: " << x << " y: " << y << std::endl;
	}
};

class Vec3i {
public:
	int x;
	int y;
	int z;

	inline Vec3i() {}

	inline Vec3i(int a, int b, int c) {
		this->x = a;
		this->y = b;
		this->z = c;
	}
};

class Vec3f {
public:
	union {
		struct {
			float x;
			float y;
			float z;
		};
		float val[3];
	};


	inline Vec3f() {
		this->x = 1.f;
		this->y = 1.f;
		this->z = 1.f;
	}

	inline Vec3f(float a, float b, float c) {
		this->x = a;
		this->y = b;
		this->z = c;
	}

	Vec3f operator+ (Vec3f v1) {
		Vec3f v;
		v.x = this->x + v1.x;
		v.y = this->y + v1.y;
		v.z = this->z + v1.z;

		return v;
	}

	Vec3f operator- (Vec3f v1) {
		Vec3f v;
		v.x = this->x - v1.x;
		v.y = this->y - v1.y;
		v.z = this->z - v1.z;

		return v;
	}

	// �˱���
	Vec3f operator* (float alpha) {
		Vec3f v;
		v.x = this->x * alpha;
		v.y = this->y * alpha;
		v.z = this->z * alpha;

		return v;
	}
	// ���
	float operator* (Vec3f v0) {
		return this->x * v0.x + this->y * v0.y + this->z * v0.z;
	}

	// cross
	Vec3f operator^(Vec3f v1) {
		Vec3f v(this->y * v1.z - this->z * v1.y,
				this->z * v1.x - this->x * v1.z,
				this->x * v1.y - this->y * v1.x);

		return v;
	}

	float& operator[] (int n) {
		return val[n];
	}

	// ������һ�����ı�����
	inline void normalize() {
		float a = std::sqrt(x * x + y * y + z * z);

		this->x /= a;
		this->y /= a;
		this->z /= a;
	}

	// ������һ�������ı���������һ����һ�����������
	inline Vec3f normalized() {
		float a = std::sqrt(x * x + y * y + z * z);

		Vec3f v(x / a, y / a, z / a);
		return v;
	}


	// ����ά�������䵽��ά
	Vec4f embed_4();
	// ��ά��3->2
	Vec2f proj_2();

	// ��ӡ����
	void printfVec() {
		std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;
	}
};

class Vec4f {
public:
	union {
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		float val[4];
	};


	inline Vec4f() {
		this->x = 1.f;
		this->y = 1.f;
		this->z = 1.f;
		this->w = 1.f;
	}

	inline Vec4f(float a, float b, float c, float d) {
		this->x = a;
		this->y = b;
		this->z = c;
		this->w = d;
	}

	Vec4f operator+ (Vec4f v1) {
		Vec4f v;
		v.x = this->x + v1.x;
		v.y = this->y + v1.y;
		v.z = this->z + v1.z;
		v.w = this->w + v1.w;

		return v;
	}

	Vec4f operator- (Vec4f v1) {
		Vec4f v;
		v.x = this->x - v1.x;
		v.y = this->y - v1.y;
		v.z = this->z - v1.z;
		v.w = this->w - v1.w;

		return v;
	}

	// �˱���
	Vec4f operator* (float alpha) {
		Vec4f v;
		v.x = this->x * alpha;
		v.y = this->y * alpha;
		v.z = this->z * alpha;
		v.w = this->w * alpha;

		return v;
	}
	// ������ -> ��β��е�ʱ��Ӧ���õõ�
	Vec4f operator/ (float alpha) {
		Vec4f v;
		v.x = this->x / alpha;
		v.y = this->y / alpha;
		v.z = this->z / alpha;
		v.w = this->w / alpha;

		return v;
	}

	// ���
	float operator* (Vec4f v0) {
		return this->x * v0.x + this->y * v0.y + this->z * v0.z + this->w * v0.w;
	}

	float& operator[] (int n) {
		return val[n];
	}

	// ������һ�����ı�������ά�����б�Ҫ��һ����
	void normalize() {
		float a = std::sqrt(x * x + y * y + z * z);

		this->x /= a;
		this->y /= a;
		this->z /= a;
	}

	// ��ά
	Vec2f proj_2();
	Vec3f proj_3();

		// ��ӡ����
	void printfVec() {
		std::cout << "x: " << x << " y: " << y << " z: " << z << " w: " << w << std::endl;
	}
};

// *******************************************************************************
// ����
// *******************************************************************************

#define DEFAULT_DIMENSIONS 4

class Matrix {
private:
	std::vector<std::vector<float>> m;
	int rows, cols;

public:
	// ��ʼ�����󣬲�����ÿ��Ԫ����Ϊ0
	Matrix(int r = DEFAULT_DIMENSIONS, int c = DEFAULT_DIMENSIONS);
	void fillMat(std::initializer_list<float> il);	// ������
	void printfMat();								// ��ӡ����
	void identity(int dimensions);					// �������Ϊ��λ����
	std::vector<float>& operator[] (int r);
	Matrix operator* (Matrix& m1);					// �������
	Vec4f operator* (Vec4f& vf);

};


// *******************************************************************************
// �任��
// *******************************************************************************
// ģ�ͱ任
Matrix ModelTrans(int world_width, int world_height, int world_depth,
	float x, float y, float z);
// �۲�任
Matrix LookAt(Vec3f camera_pos, Vec3f center, Vec3f up);
// ͶӰ -> ��Ϊͬ��ԳƵ�������
// ���� orthogonal
Matrix orthoProjection(int w, int h, int n, int f);
// ͸�� Perspective
//��1��ֱ��ȷ��������ʽ
Matrix perspProjection(int w, int h, int n, int f);
//��2��FOVʽ
Matrix perspProjection(float FOV, float aspect, int n, int f);

// ��Ļӳ��
Matrix ViewPort(int width, int height, int depth = 1);





