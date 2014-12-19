/**
 * parse.c
 */

void init_info(parse_info *p) {
  // initialize parse_info struct
}

void parse_command(char* command, struct command_type* comm) {
}

parseInfo* parse(char *cmd_line) {
  // for each cmd in cmdline {
  //  if (cmd == command) {
  //      parse_command(cmd, type)
  //      }
  //  }
}

void print_info(parse_info *info) {
  foreach type in parseInfo {
    print "type_name: type"
  }
}

void free_info(parse_info *info) {
  foreach memory_block in parseInfo {
    free(memory_block)
  }
}
