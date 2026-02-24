#ifndef __CLI_H__
#define __CLI_H__

void cli_print_help(char *name);
void cli_handle_flags(int argc, char **argv);
void cli_print_media_list(void);
void cli_print_single_media(int entry_id);

#endif
