
/* 
 * A function that executes a command using fork and execv
 */
void execute_plain(char *cmd, char *const argv[]) {

  pid_t pidChild = fork();

  if (pidChild < 0) {
    //fork failed
    fprintf(stdout, "** Command failed **\n");
    perror("fork()");
    exit(1);
  } else if (pidChild == 0) {
    //in child
    if (execv(cmd, argv) == -1) {
      //execv failed
      exit(1);
    }
  }
  //in parent
  int returnStatus;
  wait(&returnStatus);
  if (returnStatus == 0) {
    //zero exit code
    fprintf(stdout, "** Command successful **\n");
  } else {
    //non-zero exit code
    fprintf(stdout, "** Command failed **\n");
  }

}

/* 
 * A function that executes a command using fork and execv, but
 * that redirects the command's output to a file
 */
void execute_output_to_file(char *cmd, char *const argv[], char *filename) {

  pid_t pidChild = fork();

  if (pidChild < 0) {
    //fork failed
    fprintf(stderr, "** Command failed **\n");
    perror("fork()");
    exit(1);
  } else if (pidChild == 0) {
    //in child
    close(1); // close stdout (file descriptor 1)
    FILE* fp = fopen(filename, "w"); // filename gets file descriptor 1
    if (fp == NULL) {
      //fopen failed
      perror("fopen()");
      exit(1);
    }
    if (execv(cmd, argv) == -1) {
      //execv failed
      exit(1);
    }
  }
  //in parent
  int returnStatus;
  wait(&returnStatus);
  if (returnStatus == 0) {
    //zero exit code
    fprintf(stdout, "** Command successful **\n");
  } else {
    //non-zero exit code
    fprintf(stdout, "** Command failed **\n");
  }

}

/* 
 * A function that executes a command using fork and execv, but
 * that redirects the command's output to another command
 */
void execute_output_to_other(char *cmd1, char *const argv1[], char *cmd2_with_argv2) {

  FILE *out;
  int out_fd;

  //read end of the pipe
  //'out' buffer corresponds to the stdin of cmd2
  out = popen(cmd2_with_argv2, "w");
  if (out == NULL) {
    //popen failed
    fprintf(stdout, "** Command failed **\n");
    perror("popen");
    exit(1);
  }
  out_fd = fileno(out);

  //write end of the pipe
  pid_t pid = fork();
  if (pid < 0) {
    //fork failed
    fprintf(stdout, "** Command failed **\n");
    perror("fork()");
    exit(1);
  } else if (pid == 0) {
    //in child
    close(1); //close stdout (fd 1)
    dup(out_fd); //duplicate out_fd which gets fd 1
    if (execv(cmd1, argv1) == -1) {
      //execv failed
      exit(1);
    }
  }

  //in parent
  int returnStatus1, returnStatus2;
  waitpid(pid, &returnStatus1, 0); //wait for write end process
  returnStatus2 = pclose(out); //wait for read end process
  if (returnStatus1 == 0 && returnStatus2 == 0) {
    //zero exit codes
    fprintf(stdout, "** Command successful **\n");
  } else {
    //non-zero exit code
    fprintf(stdout, "** Command failed **\n");
  }

}
