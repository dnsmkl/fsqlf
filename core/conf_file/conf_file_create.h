#ifndef create_conf_file_h
#define create_conf_file_h


// Create formatting configuration file with default content.
// Return values:
//  0 on success
//  1 on failure to open
//  2 on failure to close
int create_config_file(char *config_file_name);


#endif
