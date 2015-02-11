#ifndef CONF_FILE_READ_H
#define CONF_FILE_READ_H


#include "../kw/kw.h"	// kw_conf


#define READ_SUCCESSFULL (0)
#define READ_FAILED (1)
// Read specified config file
int read_conf_file(const char *file_pathname,
					struct kw_conf * (*kw)(const char *));


// Read configuration file from default conf file
// This would be "formatting.conf" in working idrectory
// If that does not exists, then on non-windows try "~/fslqf/formatting.conf"
int read_default_conf_file(struct kw_conf * (*kw)(const char *));


#endif
