#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#define THREADSMAX (3)
#define SCHEDULING_INTERVALL (5) /* seconds */

void sigusr_handler(int signo)
{
  if (SIGUSR1 == signo)
  {
      pause();
    }
}
void * thread_function(void * pv)
{
  intptr_t iThread = (intptr_t) pv;

  pause();

    {
      int i = 0;
      for (;;)
        {
          printf("%d: %d\n", (int) iThread, i++);
          sleep(1);
        }
    }

  pthread_exit(NULL);
}

int main(int argc, char ** argv)
{
  struct sigaction signal_action;
  memset(&signal_action, 0, sizeof(signal_action));
  signal_action.sa_handler = sigusr_handler;
  sigemptyset(&signal_action.sa_mask);

  sigaction(SIGUSR1, &signal_action, NULL);
  sigaction(SIGUSR2, &signal_action, NULL);

    {
      //pthread_t threads[THREADSMAX] =
      //  { 0 };
    	pthread_t threads[THREADSMAX];
      intptr_t iThread = 0;
      /* create threads */
      for (; iThread < THREADSMAX; ++iThread)
        {
          int iResult = pthread_create(&threads[iThread], NULL, thread_function,
              (void *) iThread);
          if (iResult)
            {
              errno = iResult;
              perror("pthread_created()");
              exit(1);
            }
        }
        //for(iThread = 0; iThread< THREADSMAX; ++iThread){
        //	pthread_join(threads[iThread],NULL);
        //	printf("Joined %ld", iThread);
        //}

      sleep(1); /* Unreliable workaround: Try to make sure all threads have started and block in "pause()". See comments on how this might be fixed nicely ... */

      /* scheduling loop */
      for (iThread = 0;; ++iThread)
        {
          if (THREADSMAX == iThread)
          {
              iThread = 0;
          }
          /* Resume current thread */
          {
            int iResult = pthread_kill(threads[iThread], SIGUSR2);
            if (iResult)
            {
            	errno = iResult;
                perror("pthread_kill(..., SIGUSR2)");
                exit(2);
            }
          }
          sleep(SCHEDULING_INTERVALL);
          /* Suspend current thread */
          {
            int iResult = pthread_kill(threads[iThread], SIGUSR1);
            if (iResult)
            {
                errno = iResult;
                perror("pthread_kill(..., SIGUSR1)");
                exit(3);
            }
          }
       	}
    }

  return 0;
}
