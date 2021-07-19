#pragma once
#include<ray_tracing/myvector.h>
#include<ray_tracing/ray.h>
#include<Windows.h>
const int max_plane = 6;
class triangle
{
    //�����Σ������������ʾ
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
    �����кܶ����
    ÿ���涼������������ɵ�

    */

public:
    triangle plane[max_plane];
    int plane_number = max_plane;//�����ж��ٸ���
    int intersected_face = -1;//���ĸ�ƽ���ཻ
    bool is_intersected_with_ray = false;//�Ƿ��ཻ
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
    bool in_the_triangle(triangle t1, myvector point)//�����κ�Ҫ�жϵĵ�
    {
        /*
        ͨ�����������γɵ��������������֮���Ƿ����ԭ�������ж�
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
    bool is_intersected(ray r1)//�����ཻ����ɫ�����û���ཻ������0
    {
        /*
        ���object���ÿ�������α��棬�����Ƿ�Ӵ����˱���
        ����Ӵ����˱��棬����ture�����Ҽ�¼��Object���ĸ������ཻ�����������ཻ���ཻ������꣩
        */
        //cout << "used" << endl;
        for (int i = 0; i < max_plane; i++)//����object��ÿ��plane
        {
            bool tf = false;//һ��ʼ����Ϊֱ��û�нӴ�ƽ��
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
            //(r-a)*=0. r*n=a*n�� d=a*n, a,b,c=n
            /*
            coeffi[3] = dot(perpen,point_on_surface);
            coeffi[2] = perpen.z;
            coeffi[1] = perpen.y;
            coeffi[0] = perpen.x;
            */
            //  cout << coeffi[0] << " " << coeffi[1] << " "<<coeffi[2] << " "<<coeffi[3] << endl;
            //  Sleep(1000); 
            if (intersectionLinePlane(perpen, point_on_surface, r1))//����Ƿ��ƽ���нӴ�
            {
             //   cout << "plane_perpen:";
           //     perpen.show();
               // cout << "plane_point:";
             //   point_on_surface.show();
                touch = intersection_point_on_plane(perpen, point_on_surface, r1);//�ཻ��
                tf = true;
                //cout << "in the plane:" ;
               // touch.show();
               // Sleep(1000);
            }
            if (tf && in_the_triangle(plane[i], touch))//����ཻ����ƽ�棨�����Σ��ڲ�
            {
              //  cout << "in triangle" << endl;
                this->intersected_face = i;//�ཻ����ǵ�i��
                this->intersected_point = touch;
                //cout << "in the triangle" << endl;
                return true;//ȷ���ཻ
            }
        }
        return 0;//����͸��
    }
    double get_color()
    {
        /*
        ���ص����ཻƽ�����ɫ
        �������û�м�����ߵ�������Ҫ��is_intersected֮��ʹ��
        ͨ������ȫ�ֱ��������ܷ����ཻ�����ɫ
        Ȼ��Ҫÿ�ζ���գ������Ժ�����ߺ�object�ĸ����ཻ��ʱ�򶼻᷵����һ�����ߵļ�����
        */
        double ans;
        if (intersected_face != -1)
        {
            ans = plane[intersected_face].material;
            //intersected_face = -1;//ÿ�ζ�Ҫ���
        }
        else
        {
            return 0;//����͸��
        }
        return ans;
    }
    myvector get_point()
    {
        /*
                ���صĹ��ߺ��ཻƽ��Ľ��������
        �������û�м�����ߵ�������Ҫ��is_intersected֮��ʹ��
        ͨ������ȫ�ֱ��������ܷ����ཻ�����ɫ
        Ȼ��Ҫÿ�ζ���գ������Ժ�����ߺ�object�ĸ����ཻ��ʱ�򶼻᷵����һ�����ߵļ�����
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
            return ans;//����͸��
        }
        return ans;
    }

};