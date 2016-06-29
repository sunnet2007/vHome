

#include "CrossPlatform.h"

int InitSock()
{
	#if (defined(_WIN32) || defined(_WIN64))
	WORD wV;
	WSADATA ws;
	int er;
	wV = MAKEWORD(1, 1);
	er = WSAStartup(wV, &ws);
	if(er != 0)
	{
		return -1;
	}
	
	if(LOBYTE(ws.wVersion) != 1 || HIBYTE(ws.wVersion) != 1)
	{
		WSACleanup();
		return -1;
	}
	#endif

	return 0;
}


#if (defined(_WIN32) || defined(_WIN64))

HANDLE StartThread(LPTHREAD_START_ROUTINE fun, void * arg)
{
	return CreateThread(NULL, 0, fun, arg, 0, NULL);
}

void EndThread(int exitCode)
{
    //ExitThread(exitCode);
}

#else

pthread_t StartThread(LPTHREAD_START_ROUTINE fun, void * arg)
{
	pthread_t tid;
	if(pthread_create(&tid,NULL, fun, arg) != 0)
		return 0;

	pthread_detach(tid);
	return tid;
}


void EndThread(int exitCode)
{
    //ExitThread(exitCode);
}

#endif

#if (defined(_WIN32) || defined(_WIN64))

int MakeBackground()
{
	return 0;
}

#else

int MakeBackground()
{
    pid_t pid = fork();
    if(pid == 0)
    {
        setsid();
		/*
        chdir("/");
        umask(0);
        close(0);
        close(1);
        close(2);
		*/

		return 0;
	}
	else if(pid > 0)
	{
		exit(0);
	}
	else
	{
		return -1;
	}
}

#endif


#if (defined(_WIN32) || defined(_WIN64))

char *stristr(const char *String, const char *Pattern)
{
      char *pptr, *sptr, *start;
      unsigned int  slen, plen;

      for (start = (char *)String,
           pptr  = (char *)Pattern,
           slen  = strlen(String),
           plen  = strlen(Pattern);

           /* while string length not shorter than pattern length */

           slen >= plen;

           start++, slen--)
      {
            /* find start of pattern in string */
            while (toupper(*start) != toupper(*Pattern))
            {
                  start++;
                  slen--;

                  /* if pattern longer than string */

                  if (slen < plen)
                        return(NULL);
            }

            sptr = start;
            pptr = (char *)Pattern;

            while (toupper(*sptr) == toupper(*pptr))
            {
                  sptr++;
                  pptr++;

                  /* if end of pattern then pattern was found */

                  if ('\0' == *pptr)
                        return (start);
            }
      }
      return(NULL);
}

#endif
