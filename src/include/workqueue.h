#ifndef  WORK_QUEUE_H
#define WORK_QUEUE_H

#include <pthread.h>
#include <list>

using namespace std;

template <typename T> class WorkQueue
{
   list<T> m_queue;
   pthread_mutex_t m_mutex;
   pthread_cond_t m_condv;

public:
   WorkQueue();
   virtual ~WorkQueue();
   void add(T item);
   T remove();
   int size();
};


#endif
