

#ifndef CDBM_DATABASE_H
#define CDBM_DATABASE_H

#include "aat-incl.h"
#include "cdbm-data-model.h"

typedef struct T_cdbm_db_node {
    T_gfi_list   list_node;
    T_cdbm_value key_path;
    T_cdbm_value value;
} T_cdbm_db_node;



#endif /* CDBM_DATABASE_H */
