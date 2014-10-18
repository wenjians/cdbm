

#ifndef CDBM_DATABASE_H
#define CDBM_DATABASE_H

#include "aat-incl.h"
#include "cdbm-data-model.h"

typedef struct T_cdbm_db_node {
    T_cdbm_value key;
    T_cdbm_value val;
    T_cdbm_value def;
} T_cdbm_db_node;

#endif /* CDBM_DATABASE_H */
