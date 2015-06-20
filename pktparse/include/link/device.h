#ifndef DEVICE_H
#define DEVICE_H

#include "public/netstat.h"

Device *device_new(u_char mac[]);
void device_free(gpointer dev);
gboolean device_equal(gconstpointer a, gconstpointer b);
guint device_hash(gconstpointer a);

#endif //DEVICE_H

