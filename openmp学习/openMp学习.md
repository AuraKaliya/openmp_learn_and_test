# OpenMP学习记录

## 简单介绍

## 代码练习

* 启用openmp

    引入头文件 <omp.h>

    若成功引入，则会定义宏 _OPENMP

    ```c++
    #include <omp.h>
    
    ------------------------------------------------
    
    	std::cout << "Test1:" << std::endl;
    #if _OPENMP
    	std::cout << "support openmp! " << std::endl;
    #else
    	std::cout << " no support openmp! " << std::endl;
    #endif
    	std::cout << "Test1: end" << std::endl;
    ```

    

* 简单的并行测试

    简单的从1加到100，设置10个线程，用步长进行分区（分线程），有点类似希尔排序的思想。手动地进行划区以实现数据的隔离。

    * 设置线程数。
    * 开启并行代码区。
    * 用空间换时间，在并行区进行复杂计算。
    * 进行总汇。

    ```c++
    std::cout << "Test2:" << std::endl;
    {
    	int i = 0;
    	omp_set_num_threads(10);	//设置线程数
    	int step = 10;Ｚ
    	int sum = 0;
    	int sums[10] = { 0 };
        //开启并行代码区
    #pragma omp parallel 
    	{
    		int i, tid;
    		tid = omp_get_thread_num();
    		//std::cout << "i:" << i << " tid:" <<omp_get_thread_num()<< std::endl; 
    		for (int i = 0; i < step; ++i)
    		{
    			int number = i * step + tid;
    			printf("number:%d,tid:%d\n", i, tid);
    			sums[tid] = sums[tid] + number;　　//用空间换时间，在并行区进行复杂计算
    		}
    	}
    	for (int i = 0; i < 10; ++i) sum += sums[i];  //汇总
    	std::cout << "sum:" << sum << std::endl;
    }
    	std::cout << "Test2: end" << std::endl;
    
    ```

    

* 原子操作与通过函数执行同步

    原子操作会让执行的语句严格执行完毕，即必定会在每个线程都执行一次+=10操作，并且是独立进行的。

    最后的i的值一定是确定的。

    ```c++
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
    ```

    

* 外部私有变量

    独立开辟一个变量空间，可选择性继承外部同名变量的值。

    在step3的最后，i的值取决于最后执行完操作的线程中执行的值。

    ```c++
    std::cout << "Test4:" << std::endl;
    {
    		int i = 10;
    
    		omp_set_num_threads(10);
    #pragma omp parallel private(i)
    {
    	int tid = omp_get_thread_num();
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
    
    ```

    

* 顺序执行

    * 用for循环语句确定需要进行并行化的代码区间，并添加提示语句pragma omp parallel for ordered。
    * 在pragma  omp ordered后的语句会依照线程顺序执行。

    ```c++
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
    ```

    

* 多线程独立运行操作

    * sections表示选择性独立执行多线程操作，在相应的代码区域执行相应的代码。
    * 同时开始。

    ```c++
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
    			Sleep(2000);
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
    
    ```

    





