/*
 * Auth: Phoomintr Kaewvichien
 * Desc: Project-01, A simple shell.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <sys/errno.h>
// #include <sys/wait.h>
#include<sys/types.h>
#include <unistd.h>

// Constant needed
#define MAX_CMDLINE_SIZE 256
#define MAX_JOBS_PER_LINE 10
#define MAX_SIMPLE_CMDS 10
#define MAX_ARGS_PER_CMD 10
#define MAX_LEN 1024
extern char **environ;

// Some important variables & buffers.
char cmdline[MAX_CMDLINE_SIZE]; // For storing the input string from the
                                // user.
char *jobs[MAX_JOBS_PER_LINE];  // For storing 'job' substrings.
int seqops[MAX_SIMPLE_CMDS];    // For storing sequence operator codes
char *jobstr;                   //  for the current job being processed.
size_t count;
char *simple_cmds[MAX_SIMPLE_CMDS]; // For storing simple command strings.
char *cmd;                          // For storing the 'command word'.
char *cmdargs[MAX_ARGS_PER_CMD];    // For storing additional 'argument words'.
char *pathSla;
char *pathVar;
char *command;
void show_prompt(char *prompt);
int split_into_jobs(char *cmdline, char *jobs[], size_t count);
int scan_seqops(int seqops[], char *jobstr, size_t count);
int extract_cmd_args(char *simple_cmd, char **cmd, char *cmdargs[],
                     size_t count);
int extract_simple_cmds(char *jobstr, char *simple_cmds[], size_t count);
int readline(int fd, char *buf, int bufsz);


int readline(int fd, char *buf, int bufsz) {
  int x = 0;
  char c;
  ssize_t bytes_read;
  while (bufsz) {
    /* Just read one character */
    bytes_read = read(fd, (void *)&c, 1);
    /* Act in accordance to what happens */
    if (bytes_read < 0) {
      return bytes_read;
    } else if (bytes_read == 0) {
      return bytes_read;
    } else {
      /* We're good.  What are you going to do with 'c'?
   Maybe write it in the user's buffer!  */
      if (c == '\n') {
        buf[x] = '\0';
        x++;
        break;
      } else {
        buf[x] = c;
        x++;
      }
    }
  }

  return x;
}

void show_prompt(char *prompt) {
  printf("%s", prompt);
  fflush(stdout);
}
int split_into_jobs(char *cmdline, char *jobs[], size_t count) {
  char *delims = ";";
  int i;
  char *token;

  for (i = 0, token = strtok(cmdline, delims); token != NULL && i < count;
       token = strtok(NULL, delims), ++i) {
    jobs[i] = token;
  }

  return i;
}
int scan_seqops(int seqops[], char *jobstr, size_t count) {
  int i;
  int countsq = 0;
  for (i = 0; i < strlen(jobstr); ++i) {
    if (jobstr[i] == '&' && jobstr[i + 1] == '&') {
      seqops[countsq] = 1;
      // printf("%d", seqops[countsq]);
      ++countsq;
    }

    if (jobstr[i] == '|' && jobstr[i + 1] == '|') {
      seqops[countsq] = 2;
      // printf(" %d", seqops[countsq]);
      ++countsq;
    }
  }
  return countsq;
}

int extract_simple_cmds(char *jobstr, char *simple_cmds[], size_t count) {
  char *delims = "&&||";
  int i;
  char *token;

  for (i = 0, token = strtok(jobstr, delims); token != NULL && i < count;
       token = strtok(NULL, delims), ++i) {
    simple_cmds[i] = token;
  }

  return i;
}
int extract_cmd_args(char *simple_cmd, char **cmd, char *cmdargs[],
                     size_t count) {
  char *delims = " ";
  int i;
  char *token;
  for (i = 0, token = strtok(simple_cmd, delims); token != NULL && i < count;
       token = strtok(NULL, delims), ++i) {
    cmdargs[i] = token;
    if (i == 0) {
      *cmd = cmdargs[0];
      // printf("\t\tcmd = \"%s\"\n", cmdargs[i]);
    } else if (i > 0) {
      // printf("\t\targ[%d] = \"%s\"\n", i - 1, cmdargs[i]);
    }
  }

  if (token == NULL && i == 1) {
    // printf("\t\targ = (None supplied)\n");
  }

  return i;
}
int main(int argc, char *argv[]) {
  int bytes_read;
  int total_jobs;
  int seqops_cnt;
  int total_simple_cmds;
  int total_tokens;
  char *path;
  char *tok;
  char *delim = ":";
  int rvalx;
  char sort[MAX_CMDLINE_SIZE];
  pid_t pid, which_child;
  int child_status;
  char *cmd_pathname;
  // char *cmd_args[] = {cmd_pathname, "-l", "-F", "-h", NULL};

  // test for path
  // const char *pathname = "/usr/bin/ls";

  while (1) {
    printf("Simple Shell Phoomintr Kaewvichien\n");
    // Show the prompt to the user.
    show_prompt(">> ");
    // Read a complete line and store in buffer 'cmdline'.
    bytes_read = readline(0, cmdline, MAX_CMDLINE_SIZE);
    // printf("readline(): Got\"%s\" (rval = %d)\n", cmdline, bytes_read);

    // If we read anything (even an empty line).
    if (bytes_read > 0) {
      // Split input commandline by 'jobs'.
      total_jobs = split_into_jobs(cmdline, jobs, MAX_JOBS_PER_LINE);
      // For _each_ job...
      for (int i = 0; i < total_jobs; ++i) {
        // printf("Job #%d: \"%s\"\n", i, jobs[i]);
        //  Scan for sequence operators.
        // printf("\tseqops[] = [ ");
        seqops_cnt = scan_seqops(seqops, jobs[i], MAX_SIMPLE_CMDS);
        // printf(" ]\n");
        if (seqops_cnt > MAX_SIMPLE_CMDS) {
          printf("More seqops than MAX_SIMPLE_CMDS");
        }
        // Split the job into simple commands...
        total_simple_cmds =
            extract_simple_cmds(jobs[i], simple_cmds, MAX_SIMPLE_CMDS);

        // For _each_ simple comand...
        for (int j = 0; j < total_simple_cmds; ++j) {
          // printf("\tSimple Command #%d = \"%s\"\n", j, simple_cmds[j]);
          //  Extract the commands and the arguments.
          total_tokens =
              extract_cmd_args(simple_cmds[j], &cmd, cmdargs, MAX_ARGS_PER_CMD);

          if (total_tokens > MAX_ARGS_PER_CMD) {
            printf("More args than max_args_per_cmd");
          }
          // Check for path
          path = getenv("PATH");
          /*check if path has been retrive*/
          if (path != NULL) {
            // go thourgh every path
            for (tok = strtok(path, delim); tok != NULL;
                 tok = strtok(NULL, delim)) {
              // put slash(/), path, and command into an variable
              snprintf(sort, MAX_CMDLINE_SIZE, "%s/%s", tok, cmd);
              // printf("%s\n", sort);
              //  Check access
              rvalx = access(sort, X_OK);
              // printf("X_OK = %d\n", rvalx);
              if (rvalx == 0) {
                // printf("Found it!\n");
                break;
              }
            }
          } /*end if*/
          else {
            /*printing failure message*/
            printf("ERROR: getenv(): Unable to obtain 'PATH'.");
          }
          // fork()
          cmd_pathname = sort;
          pid = fork();
          /* First check if there was an error! */
          if (pid == -1) {
            fprintf(stderr, "ERROR: calling fork()!\n");
            exit(EXIT_FAILURE);
          }
          /* Okay, if "I am the child... " */
          if (pid == 0) {
            /* figure out what you want to do */ /* Then run a new
                                                              command/program */
            if (execve(cmd_pathname, cmdargs, environ) < 0) {
              fprintf(stderr, "ERROR: execve().\n");
              exit(EXIT_FAILURE);
            } /* end if */
          } /* end if */
          /* Otherwise, if "I am the parent..." */
          else if (pid > 0) {
            /* Let's wait for the child to finish (this will BLOCK)... */
            which_child = wait(&child_status);
            /* First check to see that wait() succeeded!
                   It could FAIL! */
            if (which_child == -1) {
              fprintf(stderr, "ERROR: wait() failed!\n");
              exit(EXIT_FAILURE);
            }

            /* Otherwise, let's assume child exited by calling 'exit()' */
            else if (WIFEXITED(child_status)) {

              /* Let's print some info... */
              fprintf(stdout,
                      "Looks like child PID %d returned with status = %d\n",
                      which_child, WEXITSTATUS(child_status));
              for (int k = 0; k < MAX_ARGS_PER_CMD; k++){
                printf("%s\n",cmdargs[k]);  
              }
              printf("%s\n",cmd_pathname);
              // exit(EXIT_SUCCESS);

            } /* end else */

          } /* end else-if() */

          // exit(EXIT_SUCCESS);
        } // end for( each simple command )
      }   // end for( each job )

    } else if (bytes_read == 0) {
      // printf("readline(): Returned with code = %d\n", bytes_read);
      // return (EXIT_SUCCESS);
      break;
    } // end if-else()

  } // end while()

  return (EXIT_SUCCESS);
}
