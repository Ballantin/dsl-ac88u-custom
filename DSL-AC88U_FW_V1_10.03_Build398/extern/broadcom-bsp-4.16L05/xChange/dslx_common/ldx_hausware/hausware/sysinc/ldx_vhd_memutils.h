#if !defined( LDX_VHD_MEMUTILS_H )
#define LDX_VHD_MEMUTILS_H

#if 0
#define  SIZEOF_LDX_HALFSOCKET  LDX_HALFSOCKET_SIZE
#define  SIZEOF_LDX_SOCKET      LDX_SOCKET_SIZE
#define  SIZEOF_KNL_THREAD      LDX_KNLTHREAD_SIZE
#define  SIZEOF_LDX_SERVICE     LDX_SERVICE_SIZE
#else
#define  SIZEOF_LDX_HALFSOCKET  sizeof( LDX_HalfSocket )
#define  SIZEOF_LDX_SOCKET      sizeof( LDX_Socket     )
#define  SIZEOF_KNL_THREAD      sizeof( KNL_Thread  )
#define  SIZEOF_LDX_SERVICE     sizeof( LDX_Service    )
#endif

/**
* Estimates the system heap memory requirement based on the type of VHD.
*
* A heavyweight heap is required for a full featured VHD.
* A lightweight heap is required for a conference VHD.
* A half VHD heap is required for a tone/endpoint VHD.
*
* A ligthweight heap can be used to accomodate a system that use either
* a conference VHD or tone VHD.
*/

/*
 * LDX_MEMCFG_FULL_VHD_STRUCT_HEAPMEM_REQ and LDX_MEMCFG_HALF_VHD_STRUCT_HEAPMEM_REQ
 * defines the memory needed to allocate LDX Socket and VHD structures. VHDs need to
 * allocate enough memory for LDX_Socket and 1 set of (KNL_Thread + LDX_HalfSocket)
 * per thread.
 */
#define  LDX_MEMCFG_FULL_VHD_STRUCT_HEAPMEM_REQ \
   ( MEM_BLOCK_SIZE( SIZEOF_LDX_SOCKET     ) + \
     MEM_BLOCK_SIZE( SIZEOF_LDX_HALFSOCKET ) + \
     MEM_BLOCK_SIZE( SIZEOF_LDX_HALFSOCKET ))


#define LDX_MEMCFG_FULL_VHD_MEM_REQ( maxNumSrv, maxInstMem, mqInSize, mqEgSize ) \
   ( MEM_TOTAL_BLOCK_SIZE( LDX_MEMCFG_FULL_VHD_STRUCT_HEAPMEM_REQ + \
                           MEM_BLOCK_SIZE( (maxNumSrv ) * SIZEOF_LDX_SERVICE ) + \
                           MEM_BLOCK_SIZE( (maxInstMem)                      ) + \
                           MEM_BLOCK_SIZE( (mqInSize + LDX_MEDQ_MAGIC_SIZE)  ) + \
                           MEM_BLOCK_SIZE( (mqEgSize + LDX_MEDQ_MAGIC_SIZE)  ) \
                         ) + \
     MEM_TOTAL_BLOCK_SIZE( SIZEOF_KNL_THREAD ) + \
     MEM_TOTAL_BLOCK_SIZE( SIZEOF_KNL_THREAD ) \
   )


#define  LDX_MEMCFG_HALF_VHD_STRUCT_HEAPMEM_REQ   \
   ( MEM_BLOCK_SIZE( SIZEOF_LDX_SOCKET ) + \
     MEM_BLOCK_SIZE( SIZEOF_LDX_HALFSOCKET ))


#define LDX_MEMCFG_HALF_VHD_MEM_REQ( maxNumSrv, maxInstMem, mqInSize, mqEgSize ) \
   ( MEM_TOTAL_BLOCK_SIZE( LDX_MEMCFG_HALF_VHD_STRUCT_HEAPMEM_REQ + \
                           MEM_BLOCK_SIZE( (maxNumSrv ) * SIZEOF_LDX_SERVICE ) + \
                           MEM_BLOCK_SIZE( (maxInstMem)                      ) + \
                           MEM_BLOCK_SIZE( (mqInSize + LDX_MEDQ_MAGIC_SIZE)                      ) + \
                           MEM_BLOCK_SIZE( (mqEgSize + LDX_MEDQ_MAGIC_SIZE)                      ) \
                         ) + \
     MEM_TOTAL_BLOCK_SIZE( SIZEOF_KNL_THREAD ) \
   )
#endif /* LDX_VHD_MEMUTILS_H */
