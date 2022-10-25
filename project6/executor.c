/*Name: Idris Akala, UID: 117000627, Directory ID: iakala */
#include <stdio.h>
#include "command.h"
#include "executor.h"
#include <sys/wait.h>
#include <sysexits.h>
#include <err.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h> 
#include <stdlib.h>
#include <fcntl.h>
#include <err.h>
#define FILE_PERMISSIONS 0664
/*static void print_tree(struct tree *t);*/
int exec_aux(struct tree *t, int fd_in, int fd_out);
int execute(struct tree *t) {
   
   exec_aux(t, STDIN_FILENO, STDOUT_FILENO);
   /* print_tree(t); */ 

   return 0;
}
int exec_aux(struct tree *t, int fd_in, int fd_out) {
   pid_t pid;
   int pipe_fd[2], status;              /* pipe array */
   fd_in = open(t->input, O_RDONLY);
   fd_out = open(t->output, O_WRONLY | O_CREAT | O_TRUNC, FILE_PERMISSIONS);
      if (t->conjunction == NONE) {
         if (strcmp(t->argv[0], "exit") == 0) {
               exit(0);
         } else if (strcmp(t->argv[0], "cd") == 0) {
               if (t->argv[1] != NULL) {
                  chdir(t->argv[1]);
               } else {
                  chdir(getenv("HOME"));
               }
         } else {
               /*Processing other commands*/
               if ((pid = fork()) < 0) {   /*Forking*/
                  err(EX_OSERR, "fork error");
               }
               if (pid != 0) { /*Parent*/
                  wait(&status);
                  return status;
               } else {    /*Child*/
                  if (t->input != NULL) {
                        if (fd_in < 0) {
                           err(EX_OSERR, "File opening (read) failed");
                        }
                        if (dup2(fd_in, STDIN_FILENO) < 0) {
                           err(EX_OSERR, "dup2 error");
                        }
                        if (close(fd_in) < 0) { /* close and prevent resource leak */
                           err(EX_OSERR, "close error");
                        }
                  }
                  if (t->output != NULL) {
                        if (fd_out < 0) {
                           err(EX_OSERR, "File opening (write) failed");
                        }
                        if (dup2(fd_out, STDOUT_FILENO) < 0) { /* Now stdout is associated with file */
                           err(EX_OSERR, "dup2 (write) failed");
                        } 
                        if (close(fd_out) < 0) { /* close and prevent resource leak */
                           err(EX_OSERR, "close error");
                        }
                  }
                  execvp(t->argv[0], t->argv);
                  fprintf(stderr, "Failed to execute %s\n", t->argv[0]);
                  exit(EX_OSERR);
               }
      }
   } else if (t->conjunction == AND) {
      if (exec_aux(t->left, fd_in, fd_out) == 0) {
         exec_aux(t->right, fd_in, fd_out);
      }
   } else if (t->conjunction == PIPE) {
      if (t->left->output != NULL) {
         printf("Ambiguous output redirect.\n");
      } else if (t->right->input != NULL) {
         printf("Ambiguous input redirect.\n");
      } else {
      if (t->input != NULL) {
         if (fd_in < 0) {
           err(EX_OSERR, "File opening (read) failed");
         }
      }
      if (t->output != NULL) {
         if (fd_out < 0) {
           err(EX_OSERR, "File opening (write) failed");
         }
      }

      if (pipe(pipe_fd) < 0) {
         err(EX_OSERR, "pipe error");
      }
      if ((pid = fork()) < 0) {   /*Forking*/
       err(EX_OSERR, "fork error");
      }
      if (pid == 0) {
         close(pipe_fd[0]);
         if (dup2(pipe_fd[1], STDOUT_FILENO) < 0) {
            err(EX_OSERR, "dup2 (write) failed");
         }
         exec_aux(t->left, fd_in, pipe_fd[1]);
         close(pipe_fd[1]);
         exit(0);
      } else if (pid != 0) {
         close(pipe_fd[1]);
         if (dup2(pipe_fd[0], STDIN_FILENO) < 0) {
            err(EX_OSERR, "dup2 (read) failed");
         }
         exec_aux(t->right, pipe_fd[0], fd_out);
         close(pipe_fd[0]);
         wait(NULL);
      }
    }
   } else if (t->conjunction == SUBSHELL) {
      if (t->input != NULL) {
         if (fd_in < 0) {
           err(EX_OSERR, "File opening (read) failed");
         }
      }
      if (t->output != NULL) {
         if (fd_out < 0) {
           err(EX_OSERR, "File opening (write) failed");
         }
      }

      if ((pid = fork()) < 0) {   /*Forking*/
       err(EX_OSERR, "fork error");
      } else {
         if (pid) {
            exec_aux(t->left, fd_in, fd_out);
            exit(0);
         } else {
            wait(NULL);
         }
      }
   }
   return 0;
}
/*static void print_tree(struct tree *t) {
   if (t != NULL) {
      print_tree(t->left);

      if (t->conjunction == NONE) {
         printf("NONE: %s, ", t->argv[0]);
      } else {
         printf("%s, ", conj[t->conjunction]);
      }
      printf("IR: %s, ", t->input);
      printf("OR: %s\n", t->output);

      print_tree(t->right);
   }
}*/

