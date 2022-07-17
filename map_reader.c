#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LINE_SIZE 512

void display_usage(char *program_name) {
  /*
   * Display program usage
   */
  printf("[-] Usage: %s [-h] -p PID\n", program_name);
}

int parse_pid_argument(int argc, char **argv) {
  /*
   * Parse CLI arguments to either get PID or print help and quit
   */
  int pid, c;
  while ((c = getopt(argc, argv, "hp:")) != -1) {
    switch (c) {
    case 'h':
      display_usage(argv[0]);
      exit(EXIT_SUCCESS);
    case 'p':
      pid = atoi(optarg);
      break;
    }
  }
  if (pid == 0) {
    printf("[-] Require a PID argument with -p\n");
    exit(EXIT_FAILURE);
  }
  return pid;
}

int main(int argc, char **argv) {
  // Parse CLI arguments
  if (argc == 1) {
    display_usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  int pid = parse_pid_argument(argc, argv);
  printf("[+] Handling PID %d\n", pid);

  // Build proc entry name
  char proc_entry[256];
  snprintf(proc_entry, 255, "/proc/%d/maps", pid);

  if (access(proc_entry, F_OK) != 0) {
    printf("[+] %s does not exist, invalid process probably\n", proc_entry);
    exit(EXIT_FAILURE);
  }
  printf("[+] Reading map entries from %s\n", proc_entry);

  // Open the file for reading
  FILE *fd = fopen(proc_entry, "r");
  if (fd == NULL) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  // Read in the file
  char *line = NULL;
  size_t line_size = 511;
  int read = 0;
  while ((read = getline(&line, &line_size, fd)) != -1) {
    if (read == 0)
      continue;
    printf("%s", line);
  }
  free(line);

  fclose(fd);
  return EXIT_SUCCESS;
}
