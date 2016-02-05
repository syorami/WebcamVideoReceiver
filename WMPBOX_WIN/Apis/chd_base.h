/*----------	2014-09-13 
 * Description:         平台系统头文件
 * Version information: Create the file
 *-----------------------------------------------------------------*/


#ifndef _CHD_BASE_H_
#define _CHD_BASE_H_

//****************************************************************
// windows 生成dll使用
#ifdef _WIN32
	#ifdef CHD_BASE_EXPORTS
		#define CHD_BASE_APIS extern "C" __declspec(dllexport)
	#else
		#define CHD_BASE_APIS extern "C" __declspec(dllexport) 
	#endif
#endif

#if defined __linux__ || __APPLE__
	#define CHD_BASE_APIS 	
#endif
//****************************************************************

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#if defined __linux__ || __APPLE__ 
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <errno.h>
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <process.h>
#endif

// 操作句柄
typedef void * chd_mem_t;	
typedef void * chd_mmu_t;
typedef void * chd_mutex_t;
typedef void * chd_sem_t;
typedef void * chd_thread_t;
typedef void * chd_cond_t;
typedef void * chd_rwlock_t;
typedef void * chd_mp_t;


#if defined __linux__ || __APPLE__
/**********************************************************************/
/*  mem    success = addr,  failure = NULL                            */
/**********************************************************************/

//线程安全分配空间， 建议在线程中malloc使用
CHD_BASE_APIS chd_mem_t chird_ts_malloc(size_t s);
//线程安全分配空间， 建议在线程中calloc使用
CHD_BASE_APIS chd_mem_t chird_ts_calloc(int cnt, size_t s);
//线程安全释放空间， 建议在线程中free使用
CHD_BASE_APIS void   chird_ts_free(chd_mem_t addr);


/**********************************************************************/
/*  mmu 内存安全分配   success = addr,  failure = NULL                */
/**********************************************************************/

#define CHD_MALLOC(size)			malloc(size)
#define CHD_FREE(p)					free(p)
#define CHD_REALLOC(old, size)		realloc(old, size)
#define CHD_MEMCPY(dest, src, size)	memcpy(dest, src, size)

typedef struct{
	int size;
	char data[0];
}CHIRD_MEM_S;

// 初始化，指定内存最大限制
CHD_BASE_APIS void chird_mmu_init(int maxsize);
// 内存分配，
CHD_BASE_APIS chd_mmu_t chird_mmu_malloc(int size);
// 内存重新分配
CHD_BASE_APIS chd_mmu_t chird_mmu_realloc(chd_mmu_t oldmmu, int size);
// 内存拷贝
CHD_BASE_APIS chd_mmu_t chird_mmu_mcpy(chd_mmu_t dest, int doffset, chd_mmu_t src, int soffset,int size);
// 内存拷贝
CHD_BASE_APIS chd_mmu_t chird_mmu_memcpy(chd_mmu_t dest, int doffset, void *src, int size);
// 获取实际的存储起始地址
CHD_BASE_APIS void * chird_mmu_getmem(chd_mmu_t mmu);
// 内存释放
CHD_BASE_APIS void chird_mmu_free(chd_mmu_t mmu);
// 内存使用情况查看
CHD_BASE_APIS void chird_mmu_traversal();

#endif

/**********************************************************************/
/*  mutex      success = 0,  failure = -1                             */
/**********************************************************************/

//创建互斥锁   0: fast  1: recursive
CHD_BASE_APIS chd_mutex_t chird_mutex_create(int nType);
//销毁互斥锁
CHD_BASE_APIS int chird_mutex_destory(chd_mutex_t mutex);
//互斥锁上锁
CHD_BASE_APIS int chird_mutex_lock(chd_mutex_t mutex);
//互斥锁尝试上锁
CHD_BASE_APIS int chird_mutex_trylock(chd_mutex_t mutex);
//互斥锁解锁
CHD_BASE_APIS int chird_mutex_unlock(chd_mutex_t mutex);

/**********************************************************************/
/*  sem        success = 0,  failure = -1                             */
/**********************************************************************/

//信号量创建
CHD_BASE_APIS chd_sem_t chird_sem_create(int value);
//信号量销毁
CHD_BASE_APIS int chird_sem_destory(chd_sem_t semid);
//发送一个信号
CHD_BASE_APIS int chird_sem_post(chd_sem_t  semid);
//等待一个信号
CHD_BASE_APIS int chird_sem_wait(chd_sem_t  semid);
//尝试等待一个信号
#if defined __linux__ || _WIN32
CHD_BASE_APIS int chird_sem_timewait(chd_sem_t semid, int s, int ns);
#endif
    
CHD_BASE_APIS int chird_sem_getvalue(chd_sem_t semid, int * value);
//信号量实现永久阻塞
CHD_BASE_APIS int chird_sem_block(void);


/**********************************************************************/
/*	thread		success = 0, failure = -1							  */
/**********************************************************************/

//线程创建
#if defined __linux__ || __APPLE__ 
CHD_BASE_APIS chd_thread_t chird_thread_create(void *(*threadFunc) (void *), void *arg, int bdetach);
#endif

#ifdef _WIN32
CHD_BASE_APIS chd_thread_t chird_thread_create(unsigned (__stdcall *threadFunc)(void *), void *arg, int bdetach );
#endif

// 线程退出
#if defined __linux__ || __APPLE__
#define chird_thread_exit(retno) pthread_exit(retno)
#endif

#ifdef _WIN32
#define chird_thread_exit(retno) _endthreadex(retno)
#endif
//CHD_BASE_APIS int chird_thread_exit(void);
// 线程等待
CHD_BASE_APIS int chird_thread_join(chd_thread_t thread);
// 线程销毁
CHD_BASE_APIS int chird_thread_destroy(chd_thread_t thread);

// 线程分离
CHD_BASE_APIS int chird_thread_detach(chd_thread_t thread);

/**********************************************************************/
/*	cond		success = 0, failure = -1							  */
/**********************************************************************/
// 创建条件变量
CHD_BASE_APIS chd_cond_t chird_cond_create(void);
// 触发条件变量
CHD_BASE_APIS int chird_cond_post(chd_cond_t cond);
// 等待条件变量
CHD_BASE_APIS int chird_cond_wait(chd_cond_t cond, int timeout);
// 销毁条件变量
CHD_BASE_APIS int chird_cond_destroy(chd_cond_t cond);

/**********************************************************************/
/*	rwlock		success = 0, failure = error code					  */
/**********************************************************************/
// 创建读写锁  rwpriority  1: 写优先级高 0: 读优先级高
CHD_BASE_APIS chd_rwlock_t chird_rwlock_create(int rwpriority);
// 销毁读写锁
CHD_BASE_APIS int chird_rwlock_destroy(chd_rwlock_t rwlock);
// 加读锁
CHD_BASE_APIS int chird_rwlock_rdlock(chd_rwlock_t rwlock);
// 加写锁
CHD_BASE_APIS int chird_rwlock_wrlock(chd_rwlock_t rwlock);
// 解锁
CHD_BASE_APIS int chird_rwlock_unlock(chd_rwlock_t rwlock);

/**********************************************************************/
/*	mempool							  */
/**********************************************************************/
CHD_BASE_APIS void * chird_mp_init(int totalsize, int pagesize);
CHD_BASE_APIS void * chird_mp_malloc(void * handle, int size);
CHD_BASE_APIS void chird_mp_free(void * handle, void * p);

#ifdef __cplusplus
}
#endif

#endif


