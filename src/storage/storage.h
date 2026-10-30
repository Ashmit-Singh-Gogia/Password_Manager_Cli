#ifndef STORAGE_H
#define STORAGE_H

#include "../common/status.h"
#include "../vault/vault.h"

Status storage_save(const vault *v, const char *path);

#endif