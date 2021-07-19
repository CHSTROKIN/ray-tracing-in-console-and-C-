#pragma once
#include<iostream>
#include<ray_tracing/myvector.h>
class ray
{
    /*
    һ������
        ������� to_point
        �з������� perpendicular

    */
public:
    myvector perpendicular;
    myvector to_point;//the point on the plane
    ray()//Ĭ��������Ǵ�0��0��ʼ���䳤��Ϊ1000������
    {
        myvector point_a(1, 0, 1);
        myvector point_b(25, 0, 10);
        myvector point_c(7, 0, 9);
        myvector ab = point_b - point_a;
        myvector bc = point_c - point_b;
        //any two line on the surface
        perpendicular = cross(ab, bc);//perpendicular
        perpendicular.normalized();
        //perpendicular = perpendicular * 1000;
        to_point.x = 0;
        to_point.y = 0;
        to_point.z = 0;
        to_point.normalized();
    }
    ray(int mx, int my)//��m��n�㷢��һ����ֱ��ƽ�������
    {
        myvector point_a(1, 1, 0);
        myvector point_b(25, 10, 0);
        //any two point on the surface
        perpendicular = cross(point_a, point_b);
        perpendicular.normalized();
        //perpendicular = perpendicular * 1000;
        to_point.x = mx;
        to_point.y = my;
        to_point.z = 0;
        //to_point.normalized();
    }
    ray(const ray& v)//replication
    {
        this->perpendicular = v.perpendicular;
        this->to_point = v.to_point;
    }
    ray(myvector v1, myvector intersect)
    {
        this->perpendicular = v1;
        this->to_point = intersect;
    }

    myvector get_point()//��������
    {
        myvector ans(0,0,0);
        ans = perpendicular + to_point;
        return ans;
    }
   
    myvector get_point(double i)
    {
        myvector ans(0, 0, 0);
        myvector n = perpendicular;
        n = n * i;
        ans = to_point +n;
        return ans;
    }

};