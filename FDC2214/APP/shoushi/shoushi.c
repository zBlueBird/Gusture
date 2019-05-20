#include "shoushi.h"
#include "FDC2214.h"
#include "time.h"
#include "oled.h"
#include "math.h"
#include "SysTick.h"
extern int flag_10s;
extern int flag_s1;
extern int flag_s2;
extern int mod;
extern char flag_e;
extern char flag_e1;
extern int max123, min123;
extern int max12345, min12345;
extern int max0[3], min0[3], mid0[3];
extern int max[5], min[5], mid[5];

int flag123 = 0;
void xunlian1()
{
    int t1 = 0;
    int sum = 0;
    int m1, m2;
    int f = 0;
    TIM_Cmd(TIM4, ENABLE);

    while (flag_10s)
    {



        t1 = dianrongzhi(0);
        if (f == 0)
        {
            m1 = t1;
            m2 = t1;
            sum = t1;
            f = 1;
        }



        if (m1 < t1)
        {
            m1 = t1;
        }
        if (m2 > t1)
        {
            m2 = t1;
        }



        sum = (sum + t1) / 2;

    }

    max0[flag_s1] = m1;
    min0[flag_s1] = m2;
    mid0[flag_s1] = sum;

    if (mid[flag_s1] < 0)
    {
        flag_e1 = 1;
    }

    TIM_Cmd(TIM4, DISABLE);
    flag_10s = 1;
    flag_s1++;

}

void error1()
{
    int a[3]  = {0, 0, 0} ;

    a[0] = jueduizhi(mid0[0], mid0[1]);
    a[1] = jueduizhi(mid0[1], mid0[2]);
    a[2] = jueduizhi(mid0[0], mid0[2]);
    if (a[0] <= err1 || a[1] <= err1 || a[2] <= err1)
    {
        flag_e = 1;
    }

}

void error2()
{
    int a[20]  = {0, 0, 0} ;
    int i, j, k = 0;
    for (i = 0; i < 5; i++)
        for (j = 0; j < 4; j++)
        {
            if (i == j)
                ;
            else
            {
                a[k] = jueduizhi(mid[i], mid[j]);
                k++;

            }

        }
    for (i = 0; i < k + 1; i++)
    {
        if (a[i] < err2)
        {

            break;
        }


    }
    if (i != k)
    {
        flag_e = 1;


    }




}

void xunlian2()
{
    int t1 = 0;
    int sum = 0;
    int m1, m2;
    int f = 0;
    TIM_Cmd(TIM4, ENABLE);
    while (flag_10s)
    {



        t1 = dianrongzhi(0);
        if (f == 0)
        {
            m1 = t1;
            m2 = t1;
            sum = t1;
            f = 1;
        }



        if (m1 < t1)
        {
            m1 = t1;
        }
        if (m2 > t1)
        {
            m2 = t1;
        }



        sum = (sum + t1) / 2;

    }

    max[flag_s2] = m1;
    min[flag_s2] = m2;
    mid[flag_s2] = sum;

    if (mid[flag_s2] < 0)
    {
        flag_e1 = 1;
    }


    TIM_Cmd(TIM4, DISABLE);
    flag_10s = 1;
    flag_s2++;

}



void fenlei1()
{
    int tempd;
    int i, j;

    max123 = maxf(max0[0], max0[1], max0[2]);
    min123 = minf(min0[0], min0[1], min0[2]);
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 2 - i; j++)
        {
            if (mid0[j] > mid0[j + 1])
            {
                tempd = mid0[j];
                mid0[j] = mid0[j + 1];
                mid0[j + 1] = tempd;

            }
        }
    }



}

void fenlei2()
{
    int tempd;
    int i = 0, j = 0;

    max12345 = 0;
    min12345 = 40000;

    while (i < 5)
    {
        if (max12345 < max[i])
        {
            max12345 = max[i];

        }
        i++;
    }

    i = 0;


    while (i < 5)
    {
        if (min12345 > min[i])
        {
            min12345 = min[i];

        }
        i++;
    }



    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4 - i; j++)
        {
            if (mid[j] > mid[j + 1])
            {
                tempd = mid[j];
                mid[j] = mid[j + 1];
                mid[j + 1] = tempd;

            }
        }
    }



}


char fangcha(int date[], int n, int f)
{
    int i;
    int sum = 0;
    int ave;

    for (i = 0; i < n; i++)
    {
        sum += date[i];
    }
    ave = sum / n;

    for (sum = i = 0; i < n; i++)
    {
        sum += pow(date[i] - ave, 2);
    }
    ave = sum / n;

    if (ave < f)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}




int panduan1(int t2)
{

    int a, b, c;

    a = jueduizhi(t2, mid0[0]) ;
    b = jueduizhi(t2, mid0[1]) ;
    c = jueduizhi(t2, mid0[2]) ;

    if (a > b)
    {
        if (b < c)
        {
            return 2;
        }
        else
        {
            return 3;
        }
    }
    else
    {
        if (a < c)
        {
            return 1;
        }
        else
        {
            return 3;
        }

    }

}



int panduan2(int t2)
{

    int a[5];
    int in;
    int i = 0 ;
    int j = 40000;
    a[0] = jueduizhi(t2, mid[0]) ;
    a[1] = jueduizhi(t2, mid[1]) ;
    a[2] = jueduizhi(t2, mid[2]) ;
    a[3] = jueduizhi(t2, mid[3]) ;
    a[4] = jueduizhi(t2, mid[4]) ;

    while (i < 5)
    {
        if (j > a[i])
        {

            j = a[i];
            in = i;
        }
        i++;
    }

    return in + 1;

}










int zuizhongpanduan()
{

    int one = 0;
    int two = 0;
    int three = 0;
    int i, j;
    int s;
    int date10[42];
    int date20[42];


    date10[0] = dianrongzhi(0);
    if (date10[0] <= (max123 + 30) && date10[0] >= (min123 - 10))
    {
        for (i = 1; i < 42; i++)
        {
            date10[i] = dianrongzhi(0);
        }
        if (fangcha(date10, 42, fc) == 1)
        {
            for (i = 0; i < 42; i++)
            {
                date20[i] = panduan1(date10[i]);
            }

            for (j = 0; j < 42; j++)
            {

                if (date20[j] == 1)
                {
                    one++;
                }
                else if (date20[j] == 2)
                {
                    two++;
                }
                else if (date20[j] == 3)
                {
                    three++;
                }
            }
            s = maxs(one, two, three);

            return s;

        }
        else
        {
            return 0;
        }

    }
    else
    {
        return 0;
    }

}


int zuizhongpanduan1()
{

    int a[6] = {0, 0, 0, 0, 0, 0};
    int i, j, in = 0;
    int s = 1;
    int f = 0;
    int date10[40];
    int date20[40];

    date10[0] = dianrongzhi(0);
    if (date10[0] <= (max12345 + 30) && date10[0] >= (min12345 - 15))
    {
        for (i = 1; i < 40; i++)
        {
            date10[i] = dianrongzhi(0);
        }
        if (fangcha(date10, 40, fc) == 1)
        {
            for (i = 0; i < 40; i++)
            {
                date20[i] = panduan2(date10[i]);
            }
            for (j = 0; j < 40; j++)
            {

                if (date20[j] == 1)
                {
                    a[1]++;
                }
                else if (date20[j] == 2)
                {
                    a[2]++;
                }
                else if (date20[j] == 3)
                {
                    a[3]++;
                }
                else if (date20[j] == 4)
                {
                    a[4]++;
                }
                else if (date20[j] == 5)
                {
                    a[5]++;
                }
            }

            while (s < 6)
            {
                if (f < a[s])
                {
                    f = a[s];
                    in = s;
                }
                s++;
            }

            return in;
        }
        else
        {
            return 0 ;
        }
    }
    else
    {
        return 0 ;
    }

}













int jueduizhi(int a, int b)
{

    int c;
    c = a - b;
    if (c < 0)

    {
        c = -c;

    }
    return c;



}

int maxf(int a, int b, int c)
{

    if (a < b)
    {
        if (b > c)
        {
            return b;
        }
        else
        {
            return c;
        }
    }
    else
    {
        if (a > c)
        {
            return a;
        }
        else
        {
            return c;
        }
    }
}



int maxs(int a, int b, int c)
{

    if (a < b)
    {
        if (b > c)
        {
            return 2;
        }
        else
        {
            return 3;
        }
    }
    else
    {
        if (a > c)
        {
            return 1;
        }
        else
        {
            return 3;
        }
    }
}



int minf(int a, int b, int c)
{

    if (a > b)
    {
        if (b < c)
        {
            return b;
        }
        else
        {
            return c;
        }
    }
    else
    {
        if (a < c)
        {
            return a;
        }
        else
        {
            return c;
        }
    }
}




