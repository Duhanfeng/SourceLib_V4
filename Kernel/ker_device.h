#ifndef  __KER_DEVICE_H
#define  __KER_DEVICE_H


#include "SourceLib.h"

#define SL_NAME_MAX	8

/**
 * device nFlags defitions
 */
#define SL_DEVICE_FLAG_DEACTIVATE       0x000           /**< device is not not initialized */

#define SL_DEVICE_FLAG_RDONLY           0x001           /**< read only */
#define SL_DEVICE_FLAG_WRONLY           0x002           /**< write only */
#define SL_DEVICE_FLAG_RDWR             0x003           /**< read and write */

#define SL_DEVICE_FLAG_REMOVABLE        0x004           /**< removable device */
#define SL_DEVICE_FLAG_STANDALONE       0x008           /**< standalone device */
#define SL_DEVICE_FLAG_ACTIVATED        0x010           /**< device is activated */
#define SL_DEVICE_FLAG_SUSPENDED        0x020           /**< device is suspended */
#define SL_DEVICE_FLAG_STREAM           0x040           /**< stream mode */

#define SL_DEVICE_CTRL_CONFIG           0x03    	/* configure device */
#define SL_DEVICE_CTRL_SET_INT          0x10    	/* enable receive irq */
#define SL_DEVICE_CTRL_CLR_INT          0x11    	/* disable receive irq */
#define SL_DEVICE_CTRL_GET_INT          0x12

#define SL_DEVICE_FLAG_INT_RX           0x100           /**< INT mode on Rx */
#define SL_DEVICE_FLAG_DMA_RX           0x200           /**< DMA mode on Rx */
#define SL_DEVICE_FLAG_INT_TX           0x400           /**< INT mode on Tx */
#define SL_DEVICE_FLAG_DMA_TX           0x800           /**< DMA mode on Tx */

#define SL_DEVICE_OFLAG_CLOSE           0x000           /**< device is closed */
#define SL_DEVICE_OFLAG_RDONLY          0x001           /**< read only access */
#define SL_DEVICE_OFLAG_WRONLY          0x002           /**< write only access */
#define SL_DEVICE_OFLAG_RDWR            0x003           /**< read and write */
#define SL_DEVICE_OFLAG_OPEN            0x008           /**< device is opened */

/**
 * general device commands
 */
#define SL_DEVICE_CTRL_RESUME           0x01            /**< resume device */
#define SL_DEVICE_CTRL_SUSPEND          0x02            /**< suspend device */

/**
 * special device commands
 */
#define SL_DEVICE_CTRL_CHAR_STREAM      0x10            /**< stream mode on char device */
#define SL_DEVICE_CTRL_BLK_GETGEOME     0x10            /**< get geometry information   */
#define SL_DEVICE_CTRL_BLK_SYNC         0x11            /**< flush data to block device */
#define SL_DEVICE_CTRL_BLK_ERASE        0x12            /**< erase block on block device */
#define SL_DEVICE_CTRL_BLK_AUTOREFRESH  0x13            /**< block device : enter/exit auto refresh mode */
#define SL_DEVICE_CTRL_NETIF_GETMAC     0x10            /**< get mac address */
#define SL_DEVICE_CTRL_MTD_FORMAT       0x10            /**< format a MTD device */
#define SL_DEVICE_CTRL_RTC_GET_TIME     0x10            /**< get time */
#define SL_DEVICE_CTRL_RTC_SET_TIME     0x11            /**< set time */
#define SL_DEVICE_CTRL_RTC_GET_ALARM    0x12            /**< get alarm */
#define SL_DEVICE_CTRL_RTC_SET_ALARM    0x13            /**< set alarm */


/**
 * Base structure of Kernel object
 */
struct sl_object
{
    char       Name[SL_NAME_MAX];                       /**< Name of kernel object */
    sl_uint8_t type;                                    /**< type of kernel object */
    sl_uint8_t flag;                                    /**< flag of kernel object */
    sl_list_t  list;                                    /**< list node of kernel object */
};


enum sl_device_class_type
{
    SL_Device_Class_Char = 0,                           /**< character device */
    SL_Device_Class_Block,                              /**< block device */
    SL_Device_Class_NetIf,                              /**< net interface */
    SL_Device_Class_MTD,                                /**< memory device */
    SL_Device_Class_CAN,                                /**< CAN device */
    SL_Device_Class_RTC,                                /**< RTC device */
    SL_Device_Class_Sound,                              /**< Sound device */
    SL_Device_Class_Graphic,                            /**< Graphic device */
    SL_Device_Class_I2CBUS,                             /**< I2C bus device */
    SL_Device_Class_USBDevice,                          /**< USB slave device */
    SL_Device_Class_USBHost,                            /**< USB host bus */
    SL_Device_Class_SPIBUS,                             /**< SPI bus device */
    SL_Device_Class_SPIDevice,                          /**< SPI device */
    SL_Device_Class_SDIO,                               /**< SDIO bus device */
    SL_Device_Class_PM,                                 /**< PM pseudo device */
    SL_Device_Class_Pipe,                               /**< Pipe device */
    SL_Device_Class_Portal,                             /**< Portal device */
    SL_Device_Class_Timer,                              /**< Timer device */
    SL_Device_Class_Miscellaneous,                      /**< Miscellaneous device */
    SL_Device_Class_Unknown                             /**< unknown device */
};



typedef struct sl_device *sl_device_t;

/**
 * Device structure
 */
typedef struct sl_device
{
    struct sl_object          parent;                   /**< inherit from sl_object */

    enum sl_device_class_type type;                     /**< device type */
    sl_uint16_t               flag;                     /**< device flag */
    sl_uint16_t               open_flag;                /**< device open flag */
    sl_uint8_t                ref_count;                /**< reference count */
    sl_uint8_t                device_id;                /**< 0 - 255 */

    /* device call back */
    sl_err_t (*rx_indicate)(sl_device_t Dev, sl_size_t Size);
    sl_err_t (*tx_complete)(sl_device_t Dev, void *Buff);

    /* common device interface */
    sl_err_t  (*Init)   (sl_device_t Dev);
    sl_err_t  (*Open)   (sl_device_t Dev, sl_uint16_t oflag);
    sl_err_t  (*Close)  (sl_device_t Dev);
    sl_size_t (*Read)   (sl_device_t Dev, sl_off_t Pos, void *Buff, sl_size_t Size);
    sl_size_t (*Write)  (sl_device_t Dev, sl_off_t Pos, const void *Buff, sl_size_t Size);
    sl_err_t  (*Control)(sl_device_t Dev, sl_uint32_t cmd, void *args);

    void                     *UserData;                /**< device private data */
}SL_DEVICE_TYPE;


/*
 * device (I/O) system interface
 */
sl_device_t SL_Device_Find(const char *Name);

sl_err_t SL_Device_Register(sl_device_t Dev, const char *Name, sl_uint16_t nFlags);
sl_err_t SL_Device_Unregister(sl_device_t Dev);
sl_err_t SL_Device_Init_all(void);

sl_err_t SL_Device_SetRxIndicate(sl_device_t Dev, sl_err_t (*rx_ind)(sl_device_t Dev, sl_size_t Size));
sl_err_t SL_Device_SetTxComplete(sl_device_t Dev, sl_err_t (*tx_done)(sl_device_t Dev, void *Buff));

sl_err_t  SL_Device_Init (sl_device_t Dev);
sl_err_t  SL_Device_Open (sl_device_t Dev, sl_uint16_t oflag);
sl_err_t  SL_Device_Close(sl_device_t Dev);
sl_size_t SL_Device_Read (sl_device_t Dev, sl_off_t Pos, void *Buff, sl_size_t Size);
sl_size_t SL_Device_Write(sl_device_t Dev, sl_off_t Pos, const void *Buff, sl_size_t Size);
sl_err_t  SL_Device_Control(sl_device_t Dev, sl_uint8_t cmd, void *arg);


#endif
