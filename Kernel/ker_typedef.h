#ifndef  __KER_TYPEDEF_H
#define  __KER_TYPEDEF_H


/* SourceLib error code definitions */
#define SL_EOK                          0               /**< There is no error */
#define SL_ERROR                        1               /**< A generic error happens */
#define SL_ETIMEOUT                     2               /**< Timed out */
#define SL_EFULL                        3               /**< The resource is full */
#define SL_EEMPTY                       4               /**< The resource is empty */
#define SL_ENOMEM                       5               /**< No memory */
#define SL_ENOSYS                       6               /**< No system */
#define SL_EBUSY                        7               /**< Busy */
#define SL_EIO                          8               /**< IO error */

#define SL_NULL                        (0)


//常用数据类型定义

/* ---数据结构--- */
typedef   signed          char  Bit8;
typedef unsigned          char  uBit8;

typedef   signed short     int  Bit16;
typedef unsigned short     int  uBit16;

typedef   signed           int  Bit32;
typedef unsigned           int  uBit32;

typedef   signed       __int64  Bit64;
typedef unsigned       __int64  uBit64;

typedef                 float   float32;
typedef                 double  double64;

typedef unsigned          char  U8;
typedef unsigned short     int  U16;
typedef unsigned           int  U32;


/* exact-width signed integer types */
typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;
typedef   signed       __int64 int64_t;

/* exact-width unsigned integer types */
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
typedef unsigned       __int64 uint64_t;


/* SourceLib basic data type definitions */
typedef int8_t                          sl_int8_t;      /**<  8bit integer type */
typedef int16_t                         sl_int16_t;     /**< 16bit integer type */
typedef int32_t                         sl_int32_t;     /**< 32bit integer type */
typedef uint8_t                         sl_uint8_t;     /**<  8bit unsigned integer type */
typedef uint16_t                        sl_uint16_t;    /**< 16bit unsigned integer type */
typedef uint32_t                        sl_uint32_t;    /**< 32bit unsigned integer type */
typedef int                             sl_bool_t;      /**< boolean type */

typedef sl_int32_t                      sl_base_t;      /**< Nbit CPU related date type */
typedef sl_uint32_t                     sl_ubase_t;     /**< Nbit unsigned CPU related data type */

typedef sl_base_t                       sl_err_t;       /**< Type for error number */
typedef sl_uint32_t                     sl_time_t;      /**< Type for time stamp */
typedef sl_uint32_t                     sl_tick_t;      /**< Type for tick count */
typedef sl_base_t                       sl_flag_t;      /**< Type for flags */
typedef sl_ubase_t                      sl_size_t;      /**< Type for size number */
typedef sl_ubase_t                      sl_dev_t;       /**< Type for device */
typedef sl_base_t                       sl_off_t;       /**< Type for offset */



#endif /* __KER_TYPEDEF_H */
