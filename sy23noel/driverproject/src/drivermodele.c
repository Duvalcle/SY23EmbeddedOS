/*
   drivermodele.c

#define    KERN_EMERG     «<0>»     system is unusable             
#define    KERN_ALERT     «<1>»      action must be taken immediately  
#define    KERN_CRIT     «<2>»      critical conditions         
#define    KERN_ERR     «<3>»     error conditions             
#define    KERN_WARNING  «<4>»     warning conditions             
#define    KERN_NOTICE «<5>»     normal but significant condition  
#define    KERN_INFO     “<6>”     informational             
#define    KERN_DEBUG     «<7>»     debug-level messages         

*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/smp_lock.h>
#include "linuxgpio.h" // pour la simulation
//#include <asm/arch/gpio.h> // pour la carte fox
#include "driverioctl.h"
#include "ioportfoxg20.h"


static int drivermodele_open(struct inode *pinode,struct file *pfile) {


  return 0;
}

static int drivermodele_release(struct inode *pinode,struct file *pfile) {

   printk(KERN_INFO"driver port Close\n");
   return 0;
}


static ssize_t drivermodele_write(struct file *pfile, const char *poctets, size_t nb,loff_t *pos) {

  return nb;
}


static ssize_t drivermodele_read(struct file *pfile, char *poctets, size_t nb,loff_t *pos) {
  int taille=0;

  return taille;
}


static long drivermodele_ioctl(struct file *pfile,unsigned int cmd,unsigned long arg) {
	
  lock_kernel();
  printk(KERN_INFO"driver port IOCTL cmd : %u\n",cmd);
  unlock_kernel();
  return 0;
}

struct file_operations drivermodele_fops = {
	owner: THIS_MODULE,
	open:drivermodele_open,
	write:drivermodele_write,
	read:drivermodele_read,
	unlocked_ioctl:drivermodele_ioctl,
	release:drivermodele_release,
};


// initialisation
static int __init drivermodele_init(void) {
	
  return 0;
}

// suppression
static void __exit drivermodele_cleanup(void) {
  
  
}


// valeurs pour modinfo : description auteur licence
MODULE_DESCRIPTION("8 bits port driver with 190 major number");
MODULE_AUTHOR("Mon nom");
MODULE_LICENSE("GPL");


module_init(drivermodele_init); // assignation de la fonction d'initialisation
module_exit(drivermodele_cleanup); // assignation de la fonction de destruction
