#include <iostream>
#include <omp.h>
#include <ctime>
#include <cstdlib>
using namespace std;


int* sum(int *a, int *b, int k)
{
    int *c = new int[k*k];
    for (int i = 0; i < k*k; i++)
        c[i] = a[i] + b[i];
    return c;
}

int* diff(int *a, int *b, int k)
{
    int* c = new int[k*k];
    for (int i = 0; i < k*k; i++)
        c[i] = a[i] - b[i];
    return c;
}

void multi_matrix(int *a, int *b, int *c, int n)
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                c[j+i*n] += a[k+i*n]*b[j+k*n];
}

void create_matrix(int *a, int k)
{
    for (int i = 0; i < k*k; i++)
    {
        a[i] = 0;
    }
}

void rand_matrix(int *a, int k)
{
    for (int i = 0; i < k*k; i++)
    {
        //srand(time(nullptr));
        int x = rand() % 100;
        a[i] = x;
    }
}
void split_matrix(int *a, int *a1, int *a2, int *a3, int *a4, int k)
{
    int t = 0;
    for (int i = 0; i < k/2; i++)
    {
        for (int j = 0; j < k/2; j++)
        {
            a1[t] = a[i*k+j];
            a2[t] = a[i*k+j+k/2];
            a3[t] = a[i*k+j+k*k/2];
            a4[t] = a[i*k+j+k*k/2+k/2];
            t++;
        }
    }
}

void collect_matrix(int *c, int *c1, int *c2, int *c3, int *c4, int k)
{
    int t = 0;
    for (int i = 0; i < k/2; i++)
    {
        for (int j = 0; j < k/2; j++)
        {
            c[i*k+j]           = c1[t];
            c[i*k+j+k/2]       = c2[t];
            c[i*k+j+k*k/2]     = c3[t];
            c[i*k+j+k*k/2+k/2] = c4[t];
            t++;
        }
    }
}

void division_matrix(int *a, int *b, int *c, int k)
{
    if (k > 64)
    {
        int *a1 = new int[k*k/4];
        int *a2 = new int[k*k/4];
        int *a3 = new int[k*k/4];
        int *a4 = new int[k*k/4];

        int *b1 = new int[k*k/4];
        int *b2 = new int[k*k/4];
        int *b3 = new int[k*k/4];
        int *b4 = new int[k*k/4];

        split_matrix(a, a1, a2, a3, a4, k);
        split_matrix(b, b1, b2, b3, b4, k);

        int *p1 = new int[k*k/4];
        int *p2 = new int[k*k/4];
        int *p3 = new int[k*k/4];
        int *p4 = new int[k*k/4];
        int *p5 = new int[k*k/4];
        int *p6 = new int[k*k/4];
        int *p7 = new int[k*k/4];

        create_matrix(p1, k/2);
        create_matrix(p2, k/2);
        create_matrix(p3, k/2);
        create_matrix(p4, k/2);
        create_matrix(p5, k/2);
        create_matrix(p6, k/2);
        create_matrix(p7, k/2);

        omp_set_num_threads(6);
        #pragma omp parallel for
        for (int i = 0; i < 7; i++)
        {
            if (i == 0)
                division_matrix(sum(a1, a4, k/2), sum(b1, b4, k/2), p1, k/2);
            if (i == 1)
                division_matrix(sum(a3, a4, k/2), b1, p2, k/2);
            if (i == 2)
                division_matrix(a1, diff(b2, b4, k/2), p3, k/2);
            if (i == 3)
                division_matrix(a4, diff(b3, b1, k/2), p4, k/2);
            if (i == 4)
                division_matrix(sum(a1, a2, k/2), b4, p5, k/2);
            if (i == 5)
                division_matrix(diff(a3, a1, k/2), sum(b1, b2, k/2), p6, k/2);
            if (i == 6)
                division_matrix(diff(a2, a4, k/2), sum(b3, b4, k/2), p7, k/2);
        }

        int *c1 = new int[k*k/4];
        int *c2 = new int[k*k/4];
        int *c3 = new int[k*k/4];
        int *c4 = new int[k*k/4];

        create_matrix(c1, k/2);
        create_matrix(c2, k/2);
        create_matrix(c3, k/2);
        create_matrix(c4, k/2);

        c1 = diff(sum(sum(p1, p4, k/2), p7, k/2), p5, k/2);
        c2 = sum(p3, p5, k/2);
        c3 = sum(p2, p4, k/2);
        c4 = sum(diff(p1, p2, k/2), sum(p3, p6, k/2), k/2);

        collect_matrix(c, c1, c2, c3, c4, k);

        delete p1;
        delete p2;
        delete p3;
        delete p4;
        delete p5;
        delete p6;
        delete p7;

        delete a1;
        delete a2;
        delete a3;
        delete a4;

        delete c1;
        delete c2;
        delete c3;
        delete c4;

        delete b1;
        delete b2;
        delete b3;
        delete b4;
    }
    else
        multi_matrix(a, b, c, k);

}

void print_mat(int n, int k, int* c)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << c[i*k+j] << ' ';
        cout << endl;
    }
    cout << endl;
}


int main()
{
    clock_t start = clock();
    int n;
    cin >> n;
    int k = 1;
    while (n > k)
        k = k * 2;
    int *a = new int[k*k];
    int *b = new int[k*k];
    rand_matrix(a, k);
    srand(time(NULL));
    rand_matrix(b, k);

    cout << "first matrix: \n";
    print_mat(n, k, a);
    cout << "second matrix: \n";
    print_mat(n, k, b);
/*
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> a[i*k+j];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        
            cin >> b[i*k+j];
        */

    int *c = new int[k*k];
    rand_matrix(c, k);
    division_matrix(a, b, c, k);
    cout << endl << endl;

   
/*
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << c[i*k+j] << ' ';
        cout << endl;
    }
*/

    cout << "final matrix: \n";
    print_mat(n, k, c);

    clock_t finish = clock();
    double exec_time = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "time = " << exec_time << endl;

    delete a;
    delete b;
    delete c;
    return 0;
}






