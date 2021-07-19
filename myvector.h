#pragma once
#include<cmath>
#include<iostream>
class myvector
{
	public:
		double x, y, z;
		myvector()
		{
			this->x = 0;
			this->y = 0;
			this->z = 0;
		}
		myvector(const myvector& v1)
		{
			this->x = v1.x;
			this->y = v1.y;
			this->z = v1.z;
		}
		myvector(double x,double y,double z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		myvector(int x, int y, int z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		double get_x()
		{
			return this->x;
		}
		double get_y()
		{
			return this->y;
		}
		double get_z()
		{
			return this->z;
		}
		double norm()
		{
			return abs(this->x) + abs(this->y) + abs(this->z);
		}
		double angle_with_origin()
		{

		}
		myvector operator*(double b)
		{
			myvector ans;
			
			ans.x=this->x * b;
			ans.y=this->y * b;
			ans.z=this->z * b;
			return ans;
		}
		myvector operator/(double b)
		{
			myvector ans;

			ans.x = this->x / b;
			ans.y = this->y / b;
			ans.z = this->z / b;
			return ans;
		}
		myvector operator+(const myvector& v2)
		{
			myvector ans;
			ans.x = this->x + v2.x;
			ans.y = this->y + v2.y;
			ans.z = this->z + v2.z;
			return ans;
		}
		myvector operator-(const myvector& v2)
		{
			myvector ans;
			ans.x = this->x - v2.x;
			ans.y = this->y - v2.y;
			ans.z = this->z - v2.z;
			return ans;
		}
		void normalized()
		{
			this->x /= this->norm();
			this->y /= this->norm();
			this->z /= this->norm();
		}
		void show()
		{
			cout << this->x << " " << this->y << " " << this->z << endl;
		}
};
double dot(myvector v1, myvector v2)
{
	return (double)((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));
}
double angle(myvector v1, myvector v2)//in radian
{
	return dot(v1, v2) / (v1.norm() * v2.norm());
}
myvector cross(myvector v1, myvector v2)
{
//(a1,a2,a3)x(b1,b2,b3)=(a2b3-a3b2,a3b1-a1b3,a1b2-a2b1)
	myvector res(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
	return res;
}