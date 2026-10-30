#ifndef STORAGE_H
#define STORAGE_H

#include "../common/status.h"
#include "../vault/vault.h"

Status storage_save(const vault *v, const char *path);
Status storage_load(vault *v, const char *path);
Status storage_get_path(char *buf, size_t size);

#endif