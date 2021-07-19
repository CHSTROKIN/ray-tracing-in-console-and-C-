// ray_tracing.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include<iostream>
#include<cstdio>
#include<windows.h>
#include<cstring>
#include<ray_tracing\egg.h>
#include<ray_tracing\myvector.h>
#include<ray_tracing\ray.h>
#include<ray_tracing\object.h>
//#include<ray_tracing\mytest.h>
using namespace std;
const int maxn = 50;
const int maxm = 50;
const int max_object = 2;
int board[maxn][maxm];
object object_list[max_object];
myvector light_source(100, 100, 100);//点光源
class render
{
    /*
    渲染器，通过模型来打印输出结果
        流程
        1：clear()清除所有的点的颜色
        2:ray_tracer()来给每个点上色
        3:键盘输入
        4：rorate()旋转目标物体
    */
public:
    void clear()
    {
        memset(board, 0, sizeof(board));
    }
    ray reflection(triangle t1, ray incident,myvector touch)//计算反射射线(t1三角形，入射光线，交汇点）
    {
        myvector ab = t1.points[1] - t1.points[0];
        myvector ac = t1.points[2] - t1.points[0];
        myvector plane_perpendicular = cross(ab, ac);//垂直于plane的直线
        myvector ray_vector;//入射光线向量
        myvector result;//反射光线的方向向量

        ray_vector = incident.perpendicular;
        plane_perpendicular.normalized();
        ray_vector.normalized();
        result=ray_vector-(plane_perpendicular*dot(ray_vector,plane_perpendicular));//计算反射向量


        ray r1(result, touch);//通过反射点和反射向量生成ray
        return r1;
    }
    void line(int x, int y, int x2, int y2)
    {
        int k = (y2 - y) / (x2 - x);
        int b = y2 - k * x2;
        for (int i = x; i <= min(x2, maxn); i++)
        {
            int z = k * i + b;
            if (z > 0)
                board[(int)i][(z % maxm)] += 1;
        }
    }
    int ray_tracer(ray from_screen, int n)//（垂直于一个平面的向量，

    {
        int color = 0;
        if (n <= 0)
        {
            return color;
        }
        for (int i = 0; i < max_object; i++)//检查所有的物体
        {
            //是否相交
            if (object_list[i].is_intersected(from_screen))//和i物体的i的平面相交
            {
                color = object_list[i].get_color();
                //cout << "intersected" << endl;
                ray reflect;
            //   int number_of_plane = 0;
                triangle plane = object_list[i].get_face();//和哪一面
                reflect = reflection(plane, from_screen,object_list[i].get_point());//计算反射光
                object_list[i].is_intersected_with_ray = false;//设置为空
                color += ray_tracer(reflect, n - 1);//递归地计算
                //暂时不支持折射效果
                //ray refract;
                //refract=refraction(object_list[i].plane[number_inter], from_screen)
                break;
            }
        }
        return color;
    }
    myvector times(double transfer[3][3], myvector v1)
    {
        double pt[3];
        for (int i = 0; i < 3; i++)
        {
            double ans = 0.0;
            ans += transfer[i][0] * v1.x;
            ans += transfer[i][1] * v1.y;
            ans += transfer[i][2] * v1.z;
            pt[i] = ans;
        }
        myvector v2;
        v2.x = pt[0];
        v2.y = pt[1];
        v2.z = pt[2];
        return v2;
    }
    void rorate(char axis, double degree)//axis in char, and degree in radian
    {
        if (axis == 'x')
        {
            double matrix[3][3]={
                {1,0,0},
                {0,cos(degree),-sin(degree)},
                {0,sin(degree),cos(degree)}
            };
            for (int i = 0; i < max_object; i++)
            {
                for (int j = 0; j < max_plane; j++)
                {
                    for(int k=0;k<3;k++)
                        object_list[i].plane[j].points[k]=times(matrix, object_list[i].plane[j].points[k]);
                }
            }
        }
        if (axis == 'y')
        {
            double matrix[3][3] = {
                {-sin(degree),0,0},
                {cos(degree),1,cos(degree)},
                {0,0,sin(degree)}
            };
            for (int i = 0; i < max_object; i++)
            {
                for (int j = 0; j < max_plane; j++)
                {
                    for (int k = 0; k < 3; k++)
                        object_list[i].plane[j].points[k] = times(matrix, object_list[i].plane[j].points[k]);
                }
            }
        }
        if (axis == 'z')
        {
            double matrix[3][3] = {
                {cos(degree),-sin(degree),0},
                {sin(degree),cos(degree),0},
                {0,0,1}
            };
            for (int i = 0; i < max_object; i++)
            {
                for (int j = 0; j < max_plane; j++)
                {
                    for (int k = 0; k < 3; k++)
                        object_list[i].plane[j].points[k] = times(matrix, object_list[i].plane[j].points[k]);
                }
            }
        }
    }
    void move(char axis, double distance)//direction and distance
    {
        if (axis == 'w')
        {
            for (int i = 0; i < max_object; i++)
            {
                for (int j = 0; j < max_plane; j++)
                {
                    for (int k = 0; k < 3; k++)
                        object_list[i].plane[j].points[k].y += 1;
                }
            }

        }
        if (axis == 'a')
        {
            for (int i = 0; i < max_object; i++)
            {
                for (int j = 0; j < max_plane; j++)
                {
                    for (int k = 0; k < 3; k++)
                        object_list[i].plane[j].points[k].x -= 1;
                }
            }
        }
        if (axis == 's')
        {
            for (int i = 0; i < max_object; i++)
            {
                for (int j = 0; j < max_plane; j++)
                {
                    for (int k = 0; k < 3; k++)
                        object_list[i].plane[j].points[k].y -= 1;
                }
            }
        }
        if (axis == 'd')
        {
            for (int i = 0; i < max_object; i++)
            {
                for (int j = 0; j < max_plane; j++)
                {
                    for (int k = 0; k < 3; k++)
                        object_list[i].plane[j].points[k].x+= 1;
                }
            }
        }
    }
    void display()
    {

        for (int i = maxn-1; i >=0; i--)
        {
            for (int j = 0; j < maxm; j++)
            {
                /**/
                ray emit = ray(j, i);
                
                //cout << "_________________________________________________" << endl;
                //cout <<"pp:" << emit.perpendicular.x << " " << emit.perpendicular.y << " " << emit.perpendicular.z << endl;
               // cout <<"to:" << emit.to_point.x << " " << emit.to_point.y << " " << emit.to_point.z << endl;
                

                board[i][j] += ray_tracer(ray(j, i), 1);//从i,j点发射的射线，最多反射n次
                int darkness = board[i][j];
               // cout << board[i][j];
                
                switch (darkness)
                {
                case 0:
                    printf(" ");
                    break;
                case 1:
                    printf("`");
                    break;
                case 2:
                    printf("~");
                    break;
                case 3:
                    printf("@");
                    break;
                default:
                    printf("#");
                    break;
                }
                
            }
            printf("\n");
        }
    }
};

void unit_test()
{
}
void add_object()
{
     /*
     
     
     */
    object_list[0].plane[0] = triangle(1, myvector(0, 0, 0), myvector(0, 10, 0), myvector(10, 0, 0));
    object_list[0].plane[1] = triangle(1, myvector(0, 10, 0), myvector(10, 10, 0), myvector(10, 0, 0));
    object_list[0].plane[2] = triangle(1, myvector(10, 0, 10), myvector(10, 0, 0), myvector(0, 0, 0));
    //object_list[0].plane[1] = triangle(1, myvector(0, 10, 0), myvector(10, 10, 0), myvector(10, 0, 0));


   // object_list[0].plane[1] = triangle(1, myvector(1, 2, 1), myvector(12, 1, 3), myvector(7, 1, 5));

}
void unit_test_1()
{
    ray r1(10, 20);
    for (int i = 0; i < 10; i++)
    {
        myvector r3;
        r3 = r1.get_point(i);
        cout << r3.x << " " << r3.y << " " << r3.z << endl;
    }
    myvector v1(1, 2, 3);
    myvector v2(2, 3, 4);
    for (int i = 0; i < 3; i++)
    {
        cout << (v1 + v2).x << " " << (v1 + v2).y << " " << (v1 + v2).z << endl;
    }
}
int main(void)
{

    //unit_test_1();
    /*
    memset(board, 0, sizeof(board));
    cout << "yes" << endl;
    render r1;
    r1.clear();
    add_object();
   // r1.line(1, 2, 10, 20);
    r1.display();
    myvector v1(1, 2, 3);
    myvector v2(2, 3, 4);
    cout << dot(v1, v2) << endl;
    myvector v3 = cross(v1, v2);
    cout << v3.x << " " << v3.y << " " << v3.z << endl;
    unit_test();
    */
    
    render r2;
    add_object();
    while (true)
    {
        r2.clear();
        r2.display();
        Sleep(1000);
        
        char c;
        cout << "input:";
        cin >> c;
        cout << endl;
        double pi = 3.1415926;
        switch (c)
        {
        case'w':
            r2.move('w', 10);
            break;
        case'a':
            r2.move('a', 10);
            break;
        case's':
            r2.move('s', 10);
            break;
        case'd':
            r2.move('d', 10);
            break;
        case'q':
            r2.rorate('y', pi/2);//not sure
            break;
        case'e':
            r2.rorate('x', pi/2);//not sure
            break;
        default:
                break;
        }
        
    }
    
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
