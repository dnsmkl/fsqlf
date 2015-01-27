#ifndef CONF_FILE_READ_H
#define CONF_FILE_READ_H


#define READ_SUCCESSFULL (0)
#define READ_FAILED (1)
// Read specified config file
int read_conf_file(const char *file_pathname);


#define FSQLF_CONFIG_FILE_NAME "formatting.conf"
// Read configuration file from default conf file
// This would be "formatting.conf" in working idrectory
// If that does not exists, then on non-windows try "~/fslqf/formatting.conf"
int read_default_conf_file();


#endif
