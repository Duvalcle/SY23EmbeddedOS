/*
   driverfox.c

NE PAS MODIFIER sauf utilisateur averti

*/


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/miscdevice.h>
#include <linux/smp_lock.h>
#include "linuxgpio.h"
#include "driverioctl.h"
#include "ioportfoxg20.h"
#include "driverfox.h"

#define BUFFMAX 50
#define DEV_MODULE "foxg20" 

static char buffer[BUFFMAX];
static unsigned int majeur,mineur;
static int nboctets,nblu;



EXPORT_SYMBOL_GPL(gpio_request);
EXPORT_SYMBOL_GPL(gpio_export);
EXPORT_SYMBOL_GPL(gpio_unexport);
EXPORT_SYMBOL_GPL(gpio_free);
EXPORT_SYMBOL_GPL(gpio_direction_input);
EXPORT_SYMBOL_GPL(gpio_direction_output);
EXPORT_SYMBOL_GPL(gpio_set_value);
EXPORT_SYMBOL_GPL(gpio_get_value);


static int requestID(unsigned id, T_GPIO *gpio, int value) {
	int trouve;
	int i;
	for(i=0,trouve=0;(i<8) && (trouve==0);i+=1) {
		if (gpio[i].kernelID == id) {
			trouve = 1;
			gpio[i].request = value;
		}
	}
	if (trouve != 0) {
		return id;
	}
	else {
      return 0;	
    }
}

static int exportID(unsigned id, T_GPIO *gpio, int export, int change) {
	int trouve;
	int i;
	for(i=0,trouve=0;(i<8) && (trouve==0);i+=1) {
		if ((gpio[i].kernelID == id)&& (gpio[i].request==INIT)) {
			trouve = 1;
			gpio[i].export = export;
		}
	}
	if (trouve != 0) {
		return id;
	}
	else {
      return 0;	
    }
}

static int inoutID(unsigned id, T_GPIO *gpio, int dir) {
	int trouve;
	int i;
	for(i=0,trouve=0;(i<8) && (trouve==0);i+=1) {
		if ((gpio[i].kernelID == id)&& (gpio[i].export==INIT)) {
			trouve = 1;
			gpio[i].dir = dir;
		}
	}
	if (trouve != 0) {
		return id;
	}
	else {
      return 0;	
    }
}

static int setvalueID(unsigned id, T_GPIO *gpio, int value) {
	int trouve;
	int i;
	for(i=0,trouve=0;(i<8) && (trouve==0);i+=1) {
		if ((gpio[i].kernelID == id)&& (gpio[i].dir==DIROUT)) {
			trouve = 1;
			gpio[i].value = value;
		}
	}
	if (trouve != 0) {
		return id;
	}
	else {
      return 0;	
    }
}

static int getvalueID(unsigned id, T_GPIO *gpio) {
	int trouve;
	int i;
	int value=-1;
	for(i=0,trouve=0;(i<8) && (trouve==0);i+=1) {
		if ((gpio[i].kernelID == id)&& (gpio[i].dir==DIRIN)) {
			trouve = 1;
			value = gpio[i].value;
		}
	}
    return value;
}

static void valuestobuffer(char *pbuffer, T_GPIO *gpio) {
	int i;
	char value;
	for(i=0;i<8;i+=1) {
		if (gpio[i].dir==DIROUT) {
			value = gpio[i].value;
			pbuffer[7-i] = gpio[i].value + '0';
		}
		else {
			pbuffer[7-i] = 'X';
		}
	}	
}

static void buffertovalues(char *pbuffer, T_GPIO *gpio) {
	int i;
	for(i=0;i<8;i+=1) {
		if (gpio[i].dir==DIRIN) {
			gpio[i].value = pbuffer[7-i] - '0';
		}
	}	
}

static void gpio_request(unsigned id,char *sysfs) {
    int r;
    int trouve;
    r=strcmp("sysfs",sysfs);
    if (r == 0) {
		printk(KERN_WARNING" driverfox Attention request %s\n",sysfs); 
	}
	else { 
        trouve = requestID(id, gpioa, INIT);
        if (trouve) {
			printk(KERN_INFO" driverfox request %d sur %s port A\n",id,sysfs); 
		}
        trouve = requestID(id, gpiob, INIT);
        if (trouve) {
			printk(KERN_INFO" driverfox request %d sur %s port B\n",id,sysfs); 
		}		
	}
}

static int gpio_export(unsigned id, int change) {
	
    int trouveA,trouveB;
    trouveA = exportID(id, gpioa, INIT ,change);
    if (trouveA) {
		printk(KERN_INFO" driverfox export %d change %d port A\n",id,change); 
	}
    trouveB = exportID(id, gpiob, INIT ,change);
    if (trouveB) {
	   printk(KERN_INFO" driverfox export %d change %d port B\n",id,change); 
	}	
	return trouveA | trouveB;
}

static void gpio_unexport(unsigned id) {
    int trouve;
    trouve = exportID(id, gpioa, NOINIT,NOINIT);
    if (trouve) {
		printk(KERN_INFO" driverfox unexport %d port A\n",id); 
	}
    trouve = exportID(id, gpiob, NOINIT ,NOINIT);
    if (trouve) {
	   printk(KERN_INFO" driverfox unexport %d port B\n",id); 
	};
}

static void gpio_free(unsigned id){
    int trouve;
    trouve = requestID(id, gpioa, NOINIT);
    if (trouve) {
			printk(KERN_INFO" driverfox free %d  port A\n",id); 
    }
    trouve = requestID(id, gpiob, NOINIT);
    if (trouve) {
			printk(KERN_INFO" driverfox free %d port B\n",id); 
	}
}

static int gpio_direction_input(unsigned id){
    int trouveA,trouveB;
    trouveA = inoutID(id, gpioa, DIRIN);
    if (trouveA) {
		printk(KERN_INFO" driverfox input %d port A\n",id); 
	}
    trouveB = inoutID(id, gpiob, DIRIN);
    if (trouveB) {
	   printk(KERN_INFO" driverfox input %d port B\n",id); 
	}	
	return trouveA | trouveB;
}

static int gpio_direction_output(unsigned id, int value){
    int trouveA,trouveB;
    trouveA = inoutID(id, gpioa, DIROUT);
    if (trouveA) {
		printk(KERN_INFO" driverfox output %d port A\n",id); 
	}
    trouveB = inoutID(id, gpiob, DIROUT);
    if (trouveB) {
	   printk(KERN_INFO" driverfox output %d port B\n",id); 
	}	
	return trouveA | trouveB;
	return 0;
}

static void gpio_set_value(unsigned id, int value){
    int trouveA,trouveB;
    trouveA = setvalueID(id, gpioa, value);
    if (trouveA) {
		printk(KERN_INFO" driverfox set value %d port A %d\n",id,value); 
	}
    trouveB = setvalueID(id, gpiob, value);
    if (trouveB) {
	   printk(KERN_INFO" driverfox set value %d port B %d\n",id,value); 
	};
	if (trouveA || trouveB) {
		nboctets = 16;
	}
}

static int gpio_get_value(unsigned id) {
    int value;
    value = getvalueID(id, gpioa);
    if (value >=0) {
		printk(KERN_INFO" driverfox get value %d port A %d\n",id,value); 
	}
	else {
     value = getvalueID(id, gpiob);
     if (value>=0) {
	   printk(KERN_INFO" driverfox get value %d port B %d\n",id,value); 
	 }
    }
	return value;
}


static int driverfox_open(struct inode *pinode,struct file *pfile) {

   printk(KERN_INFO"driverfox Open\n");
   mineur = MINOR(pinode->i_rdev);
   majeur = MAJOR(pinode->i_rdev); 
   printk(KERN_INFO"driverfox majeur %u mineur %u\n",majeur,mineur);
   nblu = 0;
   return 0;
}

static int driverfox_release(struct inode *pinode,struct file *pfile) {

   printk(KERN_INFO"driverfox Close\n");
   return 0;
}


static ssize_t driverfox_write(struct file *pfile, const char *poctets, size_t nb,loff_t *pos) {

  int nc;
  
  if (nb > BUFFMAX) {
     printk(KERN_ERR"driverfox ERREUR ecriture %d octets superieur a %d\n",(int)nb,BUFFMAX);
     return -1;
  }
  nc=copy_from_user(buffer,poctets,nb);
  buffer[nb-1]=0;
  if (nc == 0) {
   buffertovalues(buffer,gpioa);
   buffertovalues(&(buffer[8]),gpiob);	  
   nboctets=nb-1;
   printk(KERN_INFO"driverfox write %d octets : %s\n",nboctets,buffer);
  }
  else {
	  printk(KERN_ERR"driverfox erreur copie %d\n",nc); 
  }
  return nb;
}


static ssize_t driverfox_read(struct file *pfile, char *poctets, size_t nb,loff_t *pos) {

  int nc;
  if (nboctets>0) {
   printk(KERN_INFO"driverfox demande %d octets lit %d octets pos=%lld\n",nb,nboctets,*pos);
   valuestobuffer(buffer,gpioa);
   valuestobuffer(&(buffer[8]),gpiob);
   buffer[nboctets]='\n';
   nc=copy_to_user(poctets,buffer,nboctets+1);  
   buffer[nboctets]='\0';
   printk(KERN_INFO"driverfox fournit %d octets %s\n",nboctets,buffer); 
   if (nc == 0) {
     nblu = nboctets+1;
     nboctets = 0;
   }
   else {
	   printk(KERN_ERR"driverfox erreur copie %d\n",nc); 
   }
  }
  else {
	  nblu = 0;
  }
  *pos += nblu;
  printk(KERN_INFO"driverfox return %d octets pos=%lld\n",nboctets,*pos);
  return nblu;
}


static long driverfox_ioctl(struct file *pfile,unsigned int cmd,unsigned long arg) {
	
  lock_kernel();
  printk(KERN_INFO"IOCTL %u\n",cmd);
  unlock_kernel();
  return 0;
}

struct file_operations driverfox_fops = {
	owner: THIS_MODULE,
	open:driverfox_open,
	write:driverfox_write,
	read:driverfox_read,
	unlocked_ioctl:driverfox_ioctl,
	release:driverfox_release,
};

static struct miscdevice fox_dev = {
        MISC_DYNAMIC_MINOR,
        "foxg20",
        &driverfox_fops
};


// initialisation
static int __init driverfox_init(void) {
	
  int retour;	

  retour = misc_register(&fox_dev);  
  if (retour) {
	  printk(KERN_ERR"erreur enregistrement\n");
  }
  else {
	  printk(KERN_INFO"driverfox enregistrement\n");
	  nboctets=0;
  }
  
  return retour;

}

// suppression
static void __exit driverfox_cleanup(void) {

  int retour;

  retour=misc_deregister(&fox_dev);
  if (retour) {
	  printk(KERN_ERR"erreur suppression\n");
  }
  else {
	  printk(KERN_INFO"driverfox cleanup\n");
  }  
}


// valeurs pour modinfo : description auteur licence

MODULE_DESCRIPTION("driver simulation de la foxg20");
MODULE_AUTHOR("Michel Doussot");
MODULE_LICENSE("GPL");



module_init(driverfox_init); // assignation de la fonction d'initialisation
module_exit(driverfox_cleanup); // assignation de la fonction de destruction
