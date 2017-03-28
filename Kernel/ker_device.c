/*
 * File      : device.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2013, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2007-01-21     Bernard      the first version
 * 2010-05-04     Bernard      add SL_Device_Init implementation
 * 2012-10-20     Bernard      add device check in register function,
 *                             provided by Rob <rdent@iinet.net.au>
 * 2012-12-25     Bernard      return SL_EOK if the device interface not exist.
 * 2013-07-09     Grissiom     add ref_count support
 * 2016-04-02     Bernard      fix the open_flag initialization issue.
 */

#include "SourceLib.h"
#include "ker_device.h"

#define  sl_kprintf(...)    


void sl_set_errno(sl_err_t error)
{
  
}

#if 0
/**
 * This function registers a device driver with specified Name.
 *
 * @param Dev the pointer of device driver structure
 * @param Name the device driver's Name
 * @param nFlags the flag of device
 *
 * @return the error code, SL_EOK on initialization successfully.
 */
sl_err_t SL_Device_Register(sl_device_t Dev,
                            const char *Name,
                            sl_uint16_t nFlags)
{
    if (Dev == SL_NULL)
        return -SL_ERROR;

    if (SL_Device_Find(Name) != SL_NULL)
        return -SL_ERROR;

    sl_object_init(&(Dev->parent), SL_Object_Class_Device, Name);
    Dev->flag = nFlags;
    Dev->ref_count = 0;
    Dev->open_flag = 0;

    return SL_EOK;
}
//RTM_EXPORT(SL_Device_Register);

/**
 * This function removes a previously registered device driver
 *
 * @param Dev the pointer of device driver structure
 *
 * @return the error code, SL_EOK on successfully.
 */
sl_err_t SL_Device_Unregister(sl_device_t Dev)
{
    SL_ASSERT(Dev != SL_NULL);

    sl_object_detach(&(Dev->parent));

    return SL_EOK;
}
//RTM_EXPORT(SL_Device_Unregister);

/**
 * This function initializes all registered device driver
 *
 * @return the error code, SL_EOK on successfully.
 *
 * @deprecated since 1.2.x, this function is not needed because the initialization
 *             of a device is performed when applicaiton opens it.
 */
sl_err_t SL_Device_Init_all(void)
{
    return SL_EOK;
}


/**
 * This function finds a device driver by specified Name.
 *
 * @param Name the device driver's Name
 *
 * @return the registered device driver on successful, or SL_NULL on failure.
 */
sl_device_t SL_Device_Find(const char *Name)
{
    struct sl_object *object;
    struct sl_list_node *node;
    struct sl_object_information *information;

    extern struct sl_object_information sl_object_container[];

    /* enter critical */
    if (sl_thread_self() != SL_NULL)
        sl_enter_critical();

    /* try to find device object */
    information = &sl_object_container[SL_Object_Class_Device];
    for (node  = information->object_list.next;
         node != &(information->object_list);
         node  = node->next)
    {
        object = sl_list_entry(node, struct sl_object, list);
        if (strncmp(object->Name, Name, SL_NAME_MAX) == 0)
        {
            /* leave critical */
            if (sl_thread_self() != SL_NULL)
                sl_exit_critical();

            return (sl_device_t)object;
        }
    }

    /* leave critical */
    if (sl_thread_self() != SL_NULL)
        sl_exit_critical();

    /* not found */
    return SL_NULL;
}
//RTM_EXPORT(SL_Device_Find);

#endif

/**
 * This function will initialize the specified device
 *
 * @param Dev the pointer of device driver structure
 *
 * @return the result
 */
sl_err_t SL_Device_Init(sl_device_t Dev)
{
    sl_err_t result = SL_EOK;

    SL_ASSERT(Dev != SL_NULL);

    /* get device init handler */
    if (Dev->Init != SL_NULL)
    {
        if (!(Dev->flag & SL_DEVICE_FLAG_ACTIVATED))
        {
            result = Dev->Init(Dev);
            if (result != SL_EOK)
            {
                sl_kprintf("To initialize device:%s failed. The error code is %d\n",
                           Dev->parent.Name, (int)result);
            }
            else
            {
                Dev->flag |= SL_DEVICE_FLAG_ACTIVATED;
            }
        }
    }

    return result;
}

/**
 * This function will open a device
 *
 * @param Dev the pointer of device driver structure
 * @param oflag the nFlags for device open
 *
 * @return the result
 */
sl_err_t SL_Device_Open(sl_device_t Dev, sl_uint16_t oflag)
{
    sl_err_t result = SL_EOK;

    SL_ASSERT(Dev != SL_NULL);

    /* if device is not initialized, initialize it. */
    if (!(Dev->flag & SL_DEVICE_FLAG_ACTIVATED))
    {
        if (Dev->Init != SL_NULL)
        {
            result = Dev->Init(Dev);
            if (result != SL_EOK)
            {
                sl_kprintf("To initialize device:%s failed. The error code is %d\n",
                           Dev->parent.Name, result);

                return result;
            }
        }

        Dev->flag |= SL_DEVICE_FLAG_ACTIVATED;
    }

    /* device is a stand alone device and opened */
    if ((Dev->flag & SL_DEVICE_FLAG_STANDALONE) &&
        (Dev->open_flag & SL_DEVICE_OFLAG_OPEN))
    {
        return -SL_EBUSY;
    }

    /* call device open interface */
    if (Dev->Open != SL_NULL)
    {
        result = Dev->Open(Dev, oflag);
    }

    /* set open flag */
    if (result == SL_EOK || result == -SL_ENOSYS)
    {
        Dev->open_flag = oflag | SL_DEVICE_OFLAG_OPEN;

        Dev->ref_count++;
        /* don't let bad things happen silently. If you are bitten by this assert,
         * please set the ref_count to a bigger type. */
        SL_ASSERT(Dev->ref_count != 0);
    }

    return result;
}
//RTM_EXPORT(SL_Device_Open);

/**
 * This function will close a device
 *
 * @param Dev the pointer of device driver structure
 *
 * @return the result
 */
sl_err_t SL_Device_Close(sl_device_t Dev)
{
    sl_err_t result = SL_EOK;

    SL_ASSERT(Dev != SL_NULL);

    if (Dev->ref_count == 0)
        return -SL_ERROR;

    Dev->ref_count--;

    if (Dev->ref_count != 0)
        return SL_EOK;

    /* call device close interface */
    if (Dev->Close != SL_NULL)
    {
        result = Dev->Close(Dev);
    }

    /* set open flag */
    if (result == SL_EOK || result == -SL_ENOSYS)
        Dev->open_flag = SL_DEVICE_OFLAG_CLOSE;

    return result;
}
//RTM_EXPORT(SL_Device_Close);

/**
 * This function will read some data from a device.
 *
 * @param Dev the pointer of device driver structure
 * @param Pos the position of reading
 * @param Buff the data Buff to save read data
 * @param Size the Size of Buff
 *
 * @return the actually read Size on successful, otherwise negative returned.
 *
 * @note since 0.4.0, the unit of Size/Pos is a block for block device.
 */
sl_size_t SL_Device_Read(sl_device_t Dev,
                         sl_off_t    Pos,
                         void       *Buff,
                         sl_size_t   Size)
{
    SL_ASSERT(Dev != SL_NULL);

    if (Dev->ref_count == 0)
    {
        sl_set_errno(-SL_ERROR);
        return 0;
    }

    /* call device read interface */
    if (Dev->Read != SL_NULL)
    {
        return Dev->Read(Dev, Pos, Buff, Size);
    }

    /* set error code */
    sl_set_errno(-SL_ENOSYS);

    return 0;
}
//RTM_EXPORT(SL_Device_Read);

/**
 * This function will write some data to a device.
 *
 * @param Dev the pointer of device driver structure
 * @param Pos the position of written
 * @param Buff the data Buff to be written to device
 * @param Size the Size of Buff
 *
 * @return the actually written Size on successful, otherwise negative returned.
 *
 * @note since 0.4.0, the unit of Size/Pos is a block for block device.
 */
sl_size_t SL_Device_Write(sl_device_t Dev,
                          sl_off_t    Pos,
                          const void *Buff,
                          sl_size_t   Size)
{
    SL_ASSERT(Dev != SL_NULL);

    if (Dev->ref_count == 0)
    {
        sl_set_errno(-SL_ERROR);
        return 0;
    }

    /* call device write interface */
    if (Dev->Write != SL_NULL)
    {
        return Dev->Write(Dev, Pos, Buff, Size);
    }

    /* set error code */
    sl_set_errno(-SL_ENOSYS);

    return 0;
}
//RTM_EXPORT(SL_Device_Write);

/**
 * This function will perform a variety of control functions on devices.
 *
 * @param Dev the pointer of device driver structure
 * @param cmd the command sent to device
 * @param arg the argument of command
 *
 * @return the result
 */
sl_err_t SL_Device_Control(sl_device_t Dev, sl_uint8_t cmd, void *arg)
{
    SL_ASSERT(Dev != SL_NULL);

    /* call device write interface */
    if (Dev->Control != SL_NULL)
    {
        return Dev->Control(Dev, cmd, arg);
    }

    return SL_EOK;
}
//RTM_EXPORT(SL_Device_Control);

/**
 * This function will set the reception indication callback function. This callback function
 * is invoked when this device receives data.
 *
 * @param Dev the pointer of device driver structure
 * @param rx_ind the indication callback function
 *
 * @return SL_EOK
 */
sl_err_t
SL_Device_SetRxIndicate(sl_device_t Dev,
                          sl_err_t (*rx_ind)(sl_device_t Dev, sl_size_t Size))
{
    SL_ASSERT(Dev != SL_NULL);

    Dev->rx_indicate = rx_ind;

    return SL_EOK;
}
//RTM_EXPORT(SL_Device_SetRxIndicate);

/**
 * This function will set the indication callback function when device has
 * written data to physical hardware.
 *
 * @param Dev the pointer of device driver structure
 * @param tx_done the indication callback function
 *
 * @return SL_EOK
 */
sl_err_t
SL_Device_SetTxComplete(sl_device_t Dev,
                          sl_err_t (*tx_done)(sl_device_t Dev, void *Buff))
{
    SL_ASSERT(Dev != SL_NULL);

    Dev->tx_complete = tx_done;

    return SL_EOK;
}
//RTM_EXPORT(SL_Device_SetTxComplete);

