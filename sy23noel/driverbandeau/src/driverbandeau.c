/*
   driverbandeau.c

#define    KERN_EMERG     «<0>»     system is unusable             
#define    KERN_ALERT     «<1>»      action must be taken immediately  
#define    KERN_CRIT     «<2>»      critical conditions         
#define    KERN_ERR     «<3>»     error conditions             
#define    KERN_WARNING  «<4>»     warning conditions             
#define    KERN_NOTICE «<5>»     normal but significant condition  
#define    KERN_INFO     “<6>”     informational             
#define    KERN_DEBUG     «<7>»     debug-level messages         

module_param(entier, int, 0644);
module_param_array(tableau, int, NULL, 0644);
module_param_string(chaine, chaine, sizeof(chaine), 0644);

*/


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/miscdevice.h>
#include <linux/smp_lock.h>



#include "driverioctl.h"

#define BUFFMAX 500
#define DEV_MODULE "ttybandeau" 

static char buffer[BUFFMAX];
static char message[BUFFMAX];
static unsigned int majeur,mineur;
static int nboctets,nblu;
static int mode=0;
 


static int driverbandeau_open(struct inode *pinode,struct file *pfile) {

   printk(KERN_INFO"driverbandeau Open %d\n",mode);
   mineur = MINOR(pinode->i_rdev);
   majeur = MAJOR(pinode->i_rdev); 
   printk(KERN_INFO"majeur %u mineur %u\n",majeur,mineur);
   nblu = 0;
   return 0;
}

static int driverbandeau_release(struct inode *pinode,struct file *pfile) {

   printk(KERN_INFO"driverbandeau Close\n");
   return 0;
}


static ssize_t driverbandeau_write(struct file *pfile, const char *poctets, size_t nb,loff_t *pos) {

  int nc;
  
  if (nb > BUFFMAX) {
     printk(KERN_ERR"ERREUR ecriture %d octets superieur a %d\n",(int)nb,BUFFMAX);
     return -1;
  }
  
  nc=copy_from_user(buffer,poctets,nb);
  if (nc == 0) {
   nboctets=nb;
   printk(KERN_INFO"driverbandeau write %d octets\n",nboctets);
  }
  else {
	  printk(KERN_ERR"driverbandeau erreur copie %d\n",nc); 
  }
  return nb;
}

static int decode(char *buffer, char *message,int nboctets) {

	char *pcrochetouvrant;
	char *pinf;
	char *pentete;
	char *pfin;
	buffer[nboctets] = '\0';	
	pentete = strstr(buffer,"[00\\V01");
	pfin = strstr(buffer,">]");
	pinf = strchr(buffer,'<');
	pcrochetouvrant = strchr(buffer,'[');
	if ((pentete != NULL) && (pfin != NULL) && (pcrochetouvrant != NULL) && (pinf != NULL)) {
	 int lgmessage = pfin - pinf -1;
	 memcpy(message,pinf+1,lgmessage);	
	 message[lgmessage++]='\n'; 
	 return lgmessage;
    }
    else {
		 return 0;
	}
}

static ssize_t driverbandeau_read(struct file *pfile, char *poctets, size_t nb,loff_t *pos) {

  int nc;

  printk(KERN_INFO"driverbandeau demande %d octets lit %d octets pos=%lld\n",nb,nboctets,*pos);
  if (nboctets > 0) {
   if (mode == 1) {
     nc=decode(buffer,message,nboctets);
     if (nc > 0) {
       nc=copy_to_user(poctets,message,nc);
     }
   }
   else {
	 nc=copy_to_user(poctets,buffer,nboctets);  
   }  
   if (nc == 0) {
     nblu = nboctets;
     nboctets = 0;
   }
   else {
	   printk(KERN_ERR"driverbandeau erreur copie %d\n",nc); 
   }
  }
  else {
    nblu = 0;
  }
  *pos += nblu;
  printk(KERN_INFO"driverbandeau return %d octets pos=%lld\n",nboctets,*pos);
  return nblu;
}


static long driverbandeau_ioctl(struct file *pfile,unsigned int cmd,unsigned long arg) {
	
  lock_kernel();
  printk(KERN_INFO"IOCTL %u\n",cmd);
  unlock_kernel();
  return 0;
}

struct file_operations driverbandeau_fops = {
	owner: THIS_MODULE,
	open:driverbandeau_open,
	write:driverbandeau_write,
	read:driverbandeau_read,
	unlocked_ioctl:driverbandeau_ioctl,
	release:driverbandeau_release,
};

static struct miscdevice bandeau_dev = {
        MISC_DYNAMIC_MINOR,
        "ttybandeau",
        &driverbandeau_fops
};


// initialisation
static int __init driverbandeau_init(void) {
	
  int retour;	

  retour = misc_register(&bandeau_dev);  
  if (retour) {
	  printk(KERN_ERR"erreur enregistrement\n");
  }
  else {
	  printk(KERN_INFO"driverbandeau enregistrement mode : %d\n",mode);
  }
  
  return retour;

}

// suppression
static void __exit driverbandeau_cleanup(void) {

  int retour;

  retour=misc_deregister(&bandeau_dev);
  if (retour) {
	  printk(KERN_ERR"erreur suppression\n");
  }
  else {
	  printk(KERN_INFO"driverbandeau cleanup\n");
  }  
}

module_param(mode, int, 0644);

// valeurs pour modinfo : description auteur licence
MODULE_DESCRIPTION("et le restitue brut ou decode");
MODULE_DESCRIPTION("enregistre le message brut");
MODULE_DESCRIPTION("remplace le driver serie");
MODULE_DESCRIPTION("driver simulation du bandeau");
MODULE_AUTHOR("Michel Doussot");
MODULE_LICENSE("GPL");
MODULE_PARM_DESC(mode," mode 0 (brut) ou 1 (decode) : ");


module_init(driverbandeau_init); // assignation de la fonction d'initialisation
module_exit(driverbandeau_cleanup); // assignation de la fonction de destruction
