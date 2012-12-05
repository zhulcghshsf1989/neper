/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<signal.h>
#include<string.h>
#include<float.h>
#include"ut_sys.h"
#include"ut.h"

void sighandler () {}

int
ut_sys_runwtime (char* exec, char *command, double t)
{
  int status;
  pid_t pid;
  struct sigaction act;
  struct itimerval value = {{0,0},{0,0}};
  char** list = NULL;
  int qty;

  memset (&act, 0, sizeof (struct sigaction));
  act.sa_handler = sighandler;
  // act.sa_flags = SA_RESTART;

  value.it_value.tv_sec  = (long int)(t);
  value.it_value.tv_usec = (long int)((double)(t - value.it_value.tv_sec) * 1000000);

  setitimer (ITIMER_REAL, &value, 0);

  sigaction (SIGCHLD, &act, 0);
  sigaction (SIGINT, &act, 0);
  sigaction (SIGALRM, &act, 0);

  ut_string_separate (command, ' ', &list, &qty);
  list = ut_realloc_1d_pchar (list, qty + 1);
  list[qty] = NULL;
  pid = fork ();
  if (pid == 0)
  {
    execv (exec, list);
    _exit (EXIT_FAILURE);
  }
  else if (pid < 0)
    /* The fork failed. Report failure. */
    status = -1;
  else
  {
    /* printf ("waiting for pid = %d\n", pid); */
    if (waitpid (pid, &status, 0) != pid)
      status = -1;
    /* printf ("waiting for pid = %d\n", pid); */

    /* it is likely that this should not be necessary */
    if (status == -1)
    {
      /* printf ("killing %d\n", pid); */
      kill (pid, 9);
    }
  }

  ut_free_2d_char (list, qty + 1);

  return status;
}
