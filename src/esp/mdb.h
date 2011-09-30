/*
    mdb.h -- Memory Database (MED).

    Copyright (c) All Rights Reserved. See copyright notice at the bottom of the file.
 */

#ifndef _h_MDB
#define _h_MDB 1

/********************************* Includes ***********************************/

#include    "appweb.h"
#include    "edi.h"

#if BLD_FEATURE_MDB || 1

#ifdef __cplusplus
extern "C" {
#endif

/****************************** Forward Declarations **************************/

#if !DOXYGEN
#endif

/********************************** Tunables **********************************/

#define MDB_INCR    8               /**< Default memory allocation increment for MDB */

typedef struct MdbCol {
    EdiValidate     *validate;
    char            *name;
    int             type;
    int             flags;
    int             cid;
    int64           nextValue;
} MdbCol;

typedef struct MdbSchema {
    int             ncols;
    int             capacity;
    MdbCol          cols[MPR_FLEX];
} MdbSchema;

typedef struct MdbRow {
    struct MdbTable *table;
    int             rid;
    int             nfields;
    EdiValue        fields[MPR_FLEX];
} MdbRow;

typedef struct MdbTable {
    char            *name;
    MdbSchema       *schema;
    MprMutex        *mutex;
    MprHash         *index;
    MdbCol          *keyCol;
    MdbCol          *indexCol;
    MprList         *rows;
} MdbTable;

/*
    Mdb flags
 */
#define MDB_LOADING     0x1

typedef struct Mdb {
    Edi             edi;            /**< */
    int             flags;

    char            *path;          /**< Currently open database */
    MprMutex        *mutex;         /**< Multithread lock for Schema modifications only */
    MprList         *tables;        /**< List of tables */

    /*
        When loading from file
     */
    MdbTable        *loadTable;     /* Current table */
    MdbCol          *loadCol;       /* Current column */
    MdbRow          *loadRow;       /* Current row */
    int             loadField;      /* Current field */
    MprList         *loadStack;     /* State stack */
    int             loadState;      /* Current state */
    int             loadNcols;      /* Expected number of cols */
    int             lineNumber;     /* Current line number in path */
} Mdb;


#ifdef __cplusplus
} /* extern C */
#endif

#endif /* BLD_FEATURE_MDB */
#endif /* _h_MDB */

/*
    @copy   default

    Copyright (c) Embedthis Software LLC, 2003-2011. All Rights Reserved.

    This software is distributed under commercial and open source licenses.
    You may use the GPL open source license described below or you may acquire
    a commercial license from Embedthis Software. You agree to be fully bound
    by the terms of either license. Consult the LICENSE.TXT distributed with
    this software for full details.

    This software is open source; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version. See the GNU General Public License for more
    details at: http: *www.embedthis.com/downloads/gplLicense.html

    This program is distributed WITHOUT ANY WARRANTY; without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    This GPL license does NOT permit incorporating this software into
    proprietary programs. If you are unable to comply with the GPL, you must
    acquire a commercial license to use this software. Commercial licenses
    for this software and support services are available from Embedthis
    Software at http: *www.embedthis.com

    Local variables:
    tab-width: 4
    c-basic-offset: 4
    End:
    vim: sw=4 ts=4 expandtab

    @end
 */
