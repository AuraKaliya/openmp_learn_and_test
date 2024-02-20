// OpenMP_Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <omp.h>
#include <windows.h>

int main()
{
	//test1 :查看openmp是否启用
	std::cout << "Test1:" << std::endl;
#if _OPENMP
	std::cout << "support openmp! " << std::endl;
#else
	std::cout << " no support openmp! " << std::endl;
#endif

	std::cout << "Test1: end" << std::endl;
	//test1 end

	//test2  : 进行简单的并行测试  --parallel
	/*
		简单的从1加到100，设置10个线程，用步长进行分区（分线程），有点类似希尔排序的思想。
		手动地进行划区以实现数据的隔离
	*/
	std::cout << "Test2:" << std::endl;
{
	int i = 0;
	omp_set_num_threads(10);
	int step = 10;
	int sum = 0;
	int sums[10] = { 0 };
#pragma omp parallel 
	{
		int i, tid;
		tid = omp_get_thread_num();
		//std::cout << "i:" << i << " tid:" <<omp_get_thread_num()<< std::endl; 
		for (int i = 0; i < step; ++i)
		{
			int number = i * step + tid;
			printf("number:%d,tid:%d\n", i, tid);
			sums[tid] = sums[tid] + number;
		}
	}
	for (int i = 0; i < 10; ++i) sum += sums[i];
	std::cout << "sum:" << sum << std::endl;
}
	std::cout << "Test2: end" << std::endl;


	//test3 ： 原子操作与通过函数执行同步   --atomic  --barrier


	std::cout << "Test3:" << std::endl;
{
		int sum = 0;
		omp_set_num_threads(10);
#pragma omp parallel 
{
	int tid = omp_get_thread_num();

#pragma omp atomic
			sum += 10;
			//std::cout << "tid:" << tid << "   now  sum: " << sum << std::endl;
			printf("tid:%d,now sum:%d\n", tid, sum);

#pragma omp barrier
			
			
		}
		std::cout <<" sum: "<< sum<<std::endl;
	
}
	std::cout << "Test3: end" << std::endl;

	//test4 : 外部私有变量  --private  --firstprivate

	std::cout << "Test4:" << std::endl;
{
		int i = 10;

		omp_set_num_threads(10);
#pragma omp parallel private(i)
{
	
	int tid = omp_get_thread_num();
	i = tid;
	printf("1. thread %d,i=%d\n", tid,i);
	i = 200 + tid;
	printf("2. thread %d,i=%d\n", tid, i);
}
	std::cout << " step1:  i: " << i << std::endl;

#pragma omp parallel firstprivate(i)
	{
		int tid = omp_get_thread_num();
		printf("1. thread %d,i=%d\n", tid, i);
		i = 200 + tid;
		printf("2. thread %d,i=%d\n", tid, i);
	}
	std::cout << " step2:  i: " << i << std::endl;


#pragma omp parallel 
	{
		int tid = omp_get_thread_num();
		printf("1. thread %d,i=%d\n", tid, i);
		i = 200 + tid;
		printf("2. thread %d,i=%d\n", tid, i);
	}
	std::cout << " step3:  i: " << i << std::endl;

}
	std::cout << "Test4: end" << std::endl;

	//test4  : 顺序执行  --ordered 
	std::cout << "Test5:" << std::endl;
{
		int n = 10;
#pragma omp parallel for ordered
		for (int i = 0; i < n; ++i)
		{
			int tid = omp_get_thread_num();
			printf("unordered iter : %d   thread: %d \n",i,tid);
#pragma  omp ordered
			printf("ordered iter : %d  thread: %d \n",i,tid);
		}
}
	std::cout << "Test5: end" << std::endl;


	// test5 : 多线程独立运行操作   --section
	std::cout << "Test6:" << std::endl;
{
		omp_set_num_threads(4);
#pragma omp parallel sections 
	{
#pragma omp section 
		{
			int tid = omp_get_thread_num();
			printf("now this is section1,tid=%d \n",tid);
			Sleep(1000);
			printf("section1 has done\n");
		}
#pragma omp section 
		{
			int tid = omp_get_thread_num();
			printf("now this is section2,tid=%d \n", tid);
			Sleep(500);
			printf("section2 has done\n");
		}
#pragma omp section 
		{
			int tid = omp_get_thread_num();
			printf("now this is section3,tid=%d \n", tid);
			Sleep(3000);
			printf("section3 has done\n");
		}
	}


}
	std::cout << "Test6: end" << std::endl;


	//test6 :

	return 0;
}
