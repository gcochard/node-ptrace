#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/ptrace.h>
#include <signal.h>
#include <error.h>
#include <string.h>
#include <stdio.h>

#if defined(WIN32) || defined(_WIN32)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

typedef void (*callbackfp_t)(char *error, int pid, char *cookie, char *result);

EXPORT void add(int pid, char *cookie, int retries, callbackfp_t fp) {
	long r, i = 0;
    int res, stat;
	char error[2048];
	printf("start with pid %d retries %d\n", pid, retries);
	do {
		r = ptrace(PTRACE_ATTACH, pid, NULL, NULL);
		if (--retries == 0 && r < 0) { 
			//printf("childptrace.c: after reaching max entries\n");
			sprintf(error, "Reached max retries :%ld. Error is %s", i, strerror(errno));
			(*fp)(error, pid, cookie, NULL);
			return ;
		}
	} while (r == -1 && (errno == EBUSY || errno == EFAULT || errno == ESRCH));

    res = waitpid(pid, &stat, WUNTRACED);

    printf("waitpid, stat %x pid %d %d\n", stat, res, WIFSTOPPED(stat));

    if ((res != pid) || !(WIFSTOPPED(stat)))  {
        sprintf(error, "waitpid for pid %d returned error %s",  pid, strerror(errno));
        (*fp) (error, pid, cookie, NULL);
        return;
    }


	if (r < 0 ){
		sprintf(error, "Not able to attach to process %d. Error is %s", pid, strerror(errno));
		//printf("childptrace.c:: not able to attach to process");
		(*fp)(error, pid, cookie, NULL);
		return ;
	}
	ptrace (PTRACE_CONT, pid, NULL, 0);
	//printf("childptrace.c: Success in attaching to process");
	(*fp)(NULL, pid, cookie, "successfully attached to the process");
	return ; 
}


EXPORT void detach(int pid, char *cookie, int retries, callbackfp_t fp) {
	long r;
	char error[2048];

	do {
		r = ptrace(PTRACE_DETACH, pid, NULL, NULL);
		if (--retries == 0 && r < 0) { 
			//printf("childptrace.c: after reaching max entries\n");
			sprintf(error, "Reached max retries. Error is %s", strerror(errno));
			(*fp)(error, pid, cookie, NULL);
			return ;
		}
	} while (r == -1 && (errno == EBUSY || errno == EFAULT || errno == ESRCH));
	(*fp)(NULL, pid, cookie, "successfully detached from process");
	return;
}


EXPORT void getsignal(int pid, char *cookie, callbackfp_t fp){
	long r;
	int status;
	char result[2048];
	do {
        r = waitpid(pid, &status, WUNTRACED| WCONTINUED);
        if (r == -1) {
            sprintf(result, "waitpid failed due to error %s", strerror(errno));
            (*fp)(result, pid, cookie, NULL);
            return;
        }
        if (WIFEXITED(status)) {
            sprintf(result, "exited, status=%d\n", WEXITSTATUS(status));
            (*fp)(NULL, pid, cookie, "exited");
            return; 
        } else if (WIFSIGNALED(status)) {
            sprintf(result, "killed by signal %d\n", WTERMSIG(status));
            (*fp)(NULL, pid, cookie, "killed");
            return;
        } else if (WIFSTOPPED(status)) {
            sprintf(result, "stopped by signal %d\n", WSTOPSIG(status));
            //continue for SIGHUP signal
            kill (pid, SIGHUP);
	        ptrace (PTRACE_CONT, pid, NULL, 0);
            //(*fp)(NULL, pid, cookie, "stopped");
            //return;
        } else if (WIFCONTINUED(status)) {
            printf("continued\n");
        }

    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    sprintf(result, "this process is exited or signalled %d", status);
    (*fp)(NULL, pid, cookie, "perror");
    return;
}

EXPORT int sendsignal(int pid, int signal) {
	if (pid <= 0 || signal <= 0 ) 
		return 0;
    int rc;
    rc = kill (pid, signal);
	if (rc < 0) {
		printf("sendsignal: Failed to send signal to process %d, singal is %d error is %s", pid, signal, strerror(errno));
		return 0;
	}
	return 1;
} 

