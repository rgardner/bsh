#include <assert.h>
#include <stdlib.h>

#include <parse.h>
#include <types.h>

void init_info(parseInfo *p) {
  p = malloc(sizeof(parseInfo));
}

void parse_command(char* command, struct commandType *comm) {
}

parseInfo* parse(char *cmd_line) {
  assert(FALSE);
  // for each cmd in cmdline {
  //  if (cmd == command) {
  //      parse_command(cmd, type)
  //      }
  //  }
}

void print_info(parseInfo *info) {
  foreach type in parseInfo {
    print "type_name: type"
  }
}

void free_info(parse_info *info) {
  foreach memory_block in parseInfo {
    free(memory_block)
  }
}
