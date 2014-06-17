#ifndef FRAGS_H
#define FRAGS_H

#include "public/netstat.h"

IpFrag *ip_frag_new(u_int16 offset, u_char *payload, u_int16 size);
void ip_frag_free(gpointer frag);
IpFragsHdr *ip_frags_hdr_new(u_int16 identi);
void ip_frags_hdr_free(gpointer hdr);
gint ip_frags_hdr_keycmp(gconstpointer a , gconstpointer b, gpointer user_data);


#endif //FRAGS_H

