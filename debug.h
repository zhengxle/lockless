/* debug.h                                 -*- C++ -*-
   Rémi Attab (remi.attab@gmail.com), 31 Dec 2012
   FreeBSD-style copyright and disclaimer apply

   Macros to easily enable/disable debug code.
*/

#ifndef __lockless__debug_h__
#define __lockless__debug_h__

namespace lockless {


/******************************************************************************/
/* DEBUG FLAGS                                                                */
/******************************************************************************/

#ifndef LOCKLESS_LOG_DEBUG
#   define LOCKLESS_LOG_DEBUG 0
#endif

#ifndef LOCKLESS_RCU_DEBUG
#   define LOCKLESS_RCU_DEBUG 0
#endif

#ifndef LOCKLESS_MAP_DEBUG
#   define LOCKLESS_MAP_DEBUG 0
#endif

enum DebugFlags
{
    DebugLog = LOCKLESS_LOG_DEBUG,
    DebugRcu = LOCKLESS_RCU_DEBUG,
    DebugMap = LOCKLESS_MAP_DEBUG,
};


} // lockless

#endif // __lockless__debug_h__
