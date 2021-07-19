#pragma once
#include<ray_tracing/myvector.h>
#include<ray_tracing/ray.h>
#include<Windows.h>
const int max_plane = 6;
class triangle
{
    //三角形，用三个顶点表示
public:
    int material;
    myvector points[3];
    triangle()
    {
        this->material = 0;
        for (int i = 0; i < 3; i++)
        {
            this->points[i] = myvector(0, 0, 0);
        }
    }
    triangle(int m,myvector p1, myvector p2, myvector p3)
    {
        this->material = m;
        this->points[0] = p1;
        this->points[1] = p2;
        this->points[2] = p3;

    }
    void set_material()
    {
        this->material = 0;
    }
    void set_material(int n)
    {
        this->material = n;
    }
};
class object
{
    /*
    物体有很多个面
    每个面都是由三角形组成的

    */

public:
    triangle plane[max_plane];
    int plane_number = max_plane;//物体有多少个面
    int intersected_face = -1;//在哪个平面相交
    bool is_intersected_with_ray = false;//是否相交
    myvector intersected_point = myvector(-1, -1, -1);
    object()
    {
        this->plane_number = 0;
        this->is_intersected_with_ray = false;
        this->intersected_point = myvector(-1, -1, -1);
    }
    object(triangle plane[])
    {
        int size = sizeof(plane) / sizeof(plane[0]);
        this->plane_number = size;
        for (int i = 0; i < size; i++)
        {
            this->plane[i] = plane[i];
        }
        is_intersected_with_ray = false;
        intersected_point = myvector(-1, -1, -1);

    }
    bool intersectionLinePlane(myvector perpendicular, myvector point_on_plane, ray r1)
    {
        if (dot(r1.perpendicular, perpendicular) == 0)
        {
            return false;
        }
        // double t = (dot(point_on_plane, perpendicular) - dot(r1.to_point, perpendicular)) / (dot(r1.perpendicular, perpendicular));
        // this->intersected_point = r1.get_point(t);
        return true;
    }
    myvector intersection_point_on_plane(myvector perpendicular, myvector point_on_plane, ray r1)
    {
        if (dot(r1.perpendicular, perpendicular) == 0)
        {
            return myvector(-1, -1, -1);
        }
        /*
        cout << "ans_point:";
        r1.to_point.show();
        cout << "d1:" << dot(point_on_plane, perpendicular)<<endl;
        cout << "d2:" << dot(r1.to_point, perpendicular) << endl;
        */
        double t = (dot(point_on_plane, perpendicular) - dot(r1.to_point, perpendicular)) / (dot(r1.perpendicular, perpendicular));
        
        //cout <<"t:" << t << endl;
        myvector ans = r1.get_point(t);
        //cout << "ans:";
        //ans.show();
        return ans;
    }
    bool in_the_triangle(triangle t1, myvector point)//三角形和要判断的点
    {
        /*
        通过和三个点形成的三个三角形面积之和是否等于原三角形判断
        */
        bool tf = true;
        double area_oringinal = cross((t1.points[0] - t1.points[1]), (t1.points[0] - t1.points[2])).norm() * (0.5);
        double area_next, area_next_2, area_next_3;
        area_next = cross((t1.points[0] - point), (t1.points[1] - point)).norm() * (0.5);
        area_next_2 = cross((t1.points[1] - point), (t1.points[2] - point)).norm() * (0.5);
        area_next_3 = cross((t1.points[0] - point), (t1.points[2] - point)).norm() * (0.5);
       // cout << area_oringinal << " " << (area_next + area_next_2 + area_next_3) << endl;
        //Sleep(1000);
        if (area_oringinal == (area_next + area_next_2 + area_next_3))
            return true;
        else
            return false;
    }
    bool is_intersected(ray r1)//返回相交的颜色，如果没有相交，返回0
    {
        /*
        检查object里的每个三角形表面，光线是否接触到了表面
        如果接触到了表面，返回ture，并且记录和Object的哪个表面相交，且在哪里相交（相交点的坐标）
        */
        //cout << "used" << endl;
        for (int i = 0; i < max_plane; i++)//对于object的每个plane
        {
            bool tf = false;//一开始先认为直线没有接触平面
            myvector p1 = r1.get_point(1);//
            myvector p2 = r1.get_point(2);
            //    cout << p1.x << " " << p1.y << " " << p1.z << endl;
             //   Sleep(1000);
               // double coeffi[4];
            myvector touch;
            triangle surface = this->plane[i];
            myvector point_on_surface = surface.points[0];
            myvector point_two_on_surface = surface.points[1];
            myvector point_three_on_surface = surface.points[2];
            myvector ab = (point_on_surface - point_two_on_surface);
            myvector ac = (point_on_surface - point_three_on_surface);
            myvector perpen = cross(ab, ac);
            //(r-a)*=0. r*n=a*n， d=a*n, a,b,c=n
            /*
            coeffi[3] = dot(perpen,point_on_surface);
            coeffi[2] = perpen.z;
            coeffi[1] = perpen.y;
            coeffi[0] = perpen.x;
            */
            //  cout << coeffi[0] << " " << coeffi[1] << " "<<coeffi[2] << " "<<coeffi[3] << endl;
            //  Sleep(1000); 
            if (intersectionLinePlane(perpen, point_on_surface, r1))//检查是否和平面有接触
            {
             //   cout << "plane_perpen:";
           //     perpen.show();
               // cout << "plane_point:";
             //   point_on_surface.show();
                touch = intersection_point_on_plane(perpen, point_on_surface, r1);//相交点
                tf = true;
                //cout << "in the plane:" ;
               // touch.show();
               // Sleep(1000);
            }
            if (tf && in_the_triangle(plane[i], touch))//如果相交点在平面（三角形）内部
            {
              //  cout << "in triangle" << endl;
                this->intersected_face = i;//相交面就是第i面
                this->intersected_point = touch;
                //cout << "in the triangle" << endl;
                return true;//确定相交
            }
        }
        return 0;//返回透明
    }
    double get_color()
    {
        /*
        返回的是相交平面的颜色
        这个函数没有计算光线的能力，要在is_intersected之后使用
        通过调用全局变量，才能返回相交面的颜色
        然后要每次都清空，否则以后检查光线和object哪个面相交的时候都会返回上一个光线的计算结果
        */
        double ans;
        if (intersected_face != -1)
        {
            ans = plane[intersected_face].material;
            //intersected_face = -1;//每次都要清空
        }
        else
        {
            return 0;//返回透明
        }
        return ans;
    }
    myvector get_point()
    {
        /*
                返回的光线和相交平面的交点的坐标
        这个函数没有计算光线的能力，要在is_intersected之后使用
        通过调用全局变量，才能返回相交面的颜色
        然后要每次都清空，否则以后检查光线和object哪个面相交的时候都会返回上一个光线的计算结果
        */
        myvector ans(-1, -1, -1);
        ans = intersected_point;
        intersected_point = myvector(-1, -1, -1);
        return ans;
    }
    triangle get_face()
    {
        triangle ans;
        if (intersected_face != -1)
        {
            ans = plane[intersected_face];
            intersected_face = -1;
        }
        else
        {
            return ans;//返回透明
        }
        return ans;
    }

};