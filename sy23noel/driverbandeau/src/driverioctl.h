/*
   driverioctl.h
*/

#ifndef __DRIVERIOCTL_H

#define __DRIVERIOCTL_H


#define DRIVER_MAGIC 		0xF0 // choisi d'apres ioctl-number.txt
#define DRIVER_PUTSIZE 		_IOR(DRIVER_MAGIC,1,long)
#define DRIVER_RAZSIZE	 	_IO(DRIVER_MAGIC,1)
#define DRIVER_GETSIZE	 	_IOW(DRIVER_MAGIC,1,long)


#endif
