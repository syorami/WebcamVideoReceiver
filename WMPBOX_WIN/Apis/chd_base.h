/*----------	2014-09-13 
 * Description:         ƽ̨ϵͳͷ�ļ�
 * Version information: Create the file
 *-----------------------------------------------------------------*/


#ifndef _CHD_BASE_H_
#define _CHD_BASE_H_

//****************************************************************
// windows ����dllʹ��
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

// �������
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

//�̰߳�ȫ����ռ䣬 �������߳���mallocʹ��
CHD_BASE_APIS chd_mem_t chird_ts_malloc(size_t s);
//�̰߳�ȫ����ռ䣬 �������߳���callocʹ��
CHD_BASE_APIS chd_mem_t chird_ts_calloc(int cnt, size_t s);
//�̰߳�ȫ�ͷſռ䣬 �������߳���freeʹ��
CHD_BASE_APIS void   chird_ts_free(chd_mem_t addr);


/**********************************************************************/
/*  mmu �ڴ氲ȫ����   success = addr,  failure = NULL                */
/**********************************************************************/

#define CHD_MALLOC(size)			malloc(size)
#define CHD_FREE(p)					free(p)
#define CHD_REALLOC(old, size)		realloc(old, size)
#define CHD_MEMCPY(dest, src, size)	memcpy(dest, src, size)

typedef struct{
	int size;
	char data[0];
}CHIRD_MEM_S;

// ��ʼ����ָ���ڴ��������
CHD_BASE_APIS void chird_mmu_init(int maxsize);
// �ڴ���䣬
CHD_BASE_APIS chd_mmu_t chird_mmu_malloc(int size);
// �ڴ����·���
CHD_BASE_APIS chd_mmu_t chird_mmu_realloc(chd_mmu_t oldmmu, int size);
// �ڴ濽��
CHD_BASE_APIS chd_mmu_t chird_mmu_mcpy(chd_mmu_t dest, int doffset, chd_mmu_t src, int soffset,int size);
// �ڴ濽��
CHD_BASE_APIS chd_mmu_t chird_mmu_memcpy(chd_mmu_t dest, int doffset, void *src, int size);
// ��ȡʵ�ʵĴ洢��ʼ��ַ
CHD_BASE_APIS void * chird_mmu_getmem(chd_mmu_t mmu);
// �ڴ��ͷ�
CHD_BASE_APIS void chird_mmu_free(chd_mmu_t mmu);
// �ڴ�ʹ������鿴
CHD_BASE_APIS void chird_mmu_traversal();

#endif

/**********************************************************************/
/*  mutex      success = 0,  failure = -1                             */
/**********************************************************************/

//����������   0: fast  1: recursive
CHD_BASE_APIS chd_mutex_t chird_mutex_create(int nType);
//���ٻ�����
CHD_BASE_APIS int chird_mutex_destory(chd_mutex_t mutex);
//����������
CHD_BASE_APIS int chird_mutex_lock(chd_mutex_t mutex);
//��������������
CHD_BASE_APIS int chird_mutex_trylock(chd_mutex_t mutex);
//����������
CHD_BASE_APIS int chird_mutex_unlock(chd_mutex_t mutex);

/**********************************************************************/
/*  sem        success = 0,  failure = -1                             */
/**********************************************************************/

//�ź�������
CHD_BASE_APIS chd_sem_t chird_sem_create(int value);
//�ź�������
CHD_BASE_APIS int chird_sem_destory(chd_sem_t semid);
//����һ���ź�
CHD_BASE_APIS int chird_sem_post(chd_sem_t  semid);
//�ȴ�һ���ź�
CHD_BASE_APIS int chird_sem_wait(chd_sem_t  semid);
//���Եȴ�һ���ź�
#if defined __linux__ || _WIN32
CHD_BASE_APIS int chird_sem_timewait(chd_sem_t semid, int s, int ns);
#endif
    
CHD_BASE_APIS int chird_sem_getvalue(chd_sem_t semid, int * value);
//�ź���ʵ����������
CHD_BASE_APIS int chird_sem_block(void);


/**********************************************************************/
/*	thread		success = 0, failure = -1							  */
/**********************************************************************/

//�̴߳���
#if defined __linux__ || __APPLE__ 
CHD_BASE_APIS chd_thread_t chird_thread_create(void *(*threadFunc) (void *), void *arg, int bdetach);
#endif

#ifdef _WIN32
CHD_BASE_APIS chd_thread_t chird_thread_create(unsigned (__stdcall *threadFunc)(void *), void *arg, int bdetach );
#endif

// �߳��˳�
#if defined __linux__ || __APPLE__
#define chird_thread_exit(retno) pthread_exit(retno)
#endif

#ifdef _WIN32
#define chird_thread_exit(retno) _endthreadex(retno)
#endif
//CHD_BASE_APIS int chird_thread_exit(void);
// �̵߳ȴ�
CHD_BASE_APIS int chird_thread_join(chd_thread_t thread);
// �߳�����
CHD_BASE_APIS int chird_thread_destroy(chd_thread_t thread);

// �̷߳���
CHD_BASE_APIS int chird_thread_detach(chd_thread_t thread);

/**********************************************************************/
/*	cond		success = 0, failure = -1							  */
/**********************************************************************/
// ������������
CHD_BASE_APIS chd_cond_t chird_cond_create(void);
// ������������
CHD_BASE_APIS int chird_cond_post(chd_cond_t cond);
// �ȴ���������
CHD_BASE_APIS int chird_cond_wait(chd_cond_t cond, int timeout);
// ������������
CHD_BASE_APIS int chird_cond_destroy(chd_cond_t cond);

/**********************************************************************/
/*	rwlock		success = 0, failure = error code					  */
/**********************************************************************/
// ������д��  rwpriority  1: д���ȼ��� 0: �����ȼ���
CHD_BASE_APIS chd_rwlock_t chird_rwlock_create(int rwpriority);
// ���ٶ�д��
CHD_BASE_APIS int chird_rwlock_destroy(chd_rwlock_t rwlock);
// �Ӷ���
CHD_BASE_APIS int chird_rwlock_rdlock(chd_rwlock_t rwlock);
// ��д��
CHD_BASE_APIS int chird_rwlock_wrlock(chd_rwlock_t rwlock);
// ����
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


