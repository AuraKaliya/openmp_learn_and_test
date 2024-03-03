#include <iostream>
#include <omp.h>
#include <vector>

int N = 100000000;
//int N = 100;

//����Ӻ�

void func()
{
    //1 . �� reduction ��Լ

    int j = 0;
    double tavg = 0;

    double timer_start = omp_get_wtime();
    omp_set_num_threads(48);

#pragma omp parallel for reduction(+:tavg)
        for (j = 0; j < N; ++j) {
            tavg += j;
        }
    double timer_elapsed = omp_get_wtime() - timer_start;

    std::cout << tavg << " took " << timer_elapsed << std::endl;
}

void func2()
{
   //2 . for ���� �ڼӺʹ�����ԭ�Ӳ���
    int j = 0;
    double tavg = 0;

    double timer_start = omp_get_wtime();
    omp_set_num_threads(48);

#pragma omp parallel for 
            for (j = 0; j < N; ++j) {
#pragma omp atomic
                tavg += j;
            }
   
    double timer_elapsed = omp_get_wtime() - timer_start;

    std::cout << tavg << " took " << timer_elapsed << std::endl;
}

void func3()
{
    //3. ֱ�Ӽ���
    double sum = 0;
    double timer_start = omp_get_wtime();

    for (int i = 0; i < N; ++i)
        sum += i;

    double timer_elapsed = omp_get_wtime() - timer_start;

    std::cout << sum << " took " << timer_elapsed << std::endl;
}

void func4()
{
    //4  ����洢 ���Ӻ�
    int j = 0;
    int threads = 48;
    std::vector<double>tavg = std::vector<double>(threads);
    //double tavg[threads] = {0};
    double sum = 0;
    omp_set_num_threads(threads);
    double timer_start = omp_get_wtime();
#pragma omp parallel private(j)
    {
        for (j = omp_get_thread_num(); j < N; j += omp_get_num_threads()) {
            tavg[omp_get_thread_num()] += j;
        }
    }
    
    for (int i = 0; i < threads; ++i)
    {
        sum += tavg[i];
    }

    double timer_elapsed = omp_get_wtime() - timer_start;

    std::cout << sum << " took " << timer_elapsed << std::endl;
}




void func5()
{
    //4  ���м��� atomic�Ӻ�
    int j = 0;
    int threads = 48;
    double tavg = 0;
    double sum = 0;
    omp_set_num_threads(threads);
    double timer_start = omp_get_wtime();
#pragma omp parallel private(j),firstprivate(tavg)
    {
        for (j = omp_get_thread_num(); j < N; j += omp_get_num_threads()) {
            tavg += j;
        }
#pragma omp  atomic
        sum += tavg;
    }
    double timer_elapsed = omp_get_wtime() - timer_start;

    std::cout << sum << " took " << timer_elapsed << std::endl;
}
int main() {
    func();
    std::cout << "----------------------" << std::endl;
    func2();
    std::cout << "----------------------" << std::endl;
    func3();
    std::cout << "----------------------" << std::endl;
    func4();
    std::cout << "----------------------" << std::endl;
    func5();
	return 0;
}