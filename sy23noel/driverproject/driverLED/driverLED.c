/*
   driverLED.c

#define    KERN_EMERG     «<0>»     system is unusable             
#define    KERN_ALERT     «<1>»      action must be taken immediately  
#define    KERN_CRIT     «<2>»      critical conditions         
#define    KERN_ERR     «<3>»     error conditions             
#define    KERN_WARNING  «<4>»     warning conditions             
#define    KERN_NOTICE «<5>»     normal but significant condition  
#define    KERN_INFO     «<6>»    informational             
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

#define BUFFMAX 50
static char buffer[BUFFMAX];
static unsigned int majeur=190, mineur;
static int nboctets=9,nblu;
//Tab for keeping in memory kernel id's of GPIO Port A and B
static int GPIO_PORTA[8] = {FOXG20_KID_PA7,FOXG20_KID_PA6,FOXG20_KID_PA5,FOXG20_KID_PA4,FOXG20_KID_PA3,FOXG20_KID_PA2,FOXG20_KID_PA1,FOXG20_KID_PA0};
static int GPIO_PORTB[8] = {FOXG20_KID_PB7,FOXG20_KID_PB6,FOXG20_KID_PB5,FOXG20_KID_PB4,FOXG20_KID_PB3,FOXG20_KID_PB2,FOXG20_KID_PB1,FOXG20_KID_PB0};

static int driverLED_open(struct inode *pinode,struct file *pfile) {
	printk(KERN_INFO"driverLED Open\n");
	mineur = MINOR(pinode->i_rdev);
	majeur = MAJOR(pinode->i_rdev);
	printk(KERN_INFO"majeur %u mineur %u \n",majeur,mineur);
	return 0;
}

static int driverLED_release(struct inode *pinode,struct file *pfile) {
	
   printk(KERN_INFO"driverLED port Close\n");
   return 0;
}


static ssize_t driverLED_write(struct file *pfile, const char *poctets, size_t nb,loff_t *pos) {
	int i;
	//SET VALUE PORT B
	for (i = 0; i<8 ;i++){
		gpio_direction_output(GPIO_PORTB[i] ,0); // Port B bit i
	}
	copy_from_user(buffer,poctets,8	); //Copy buffer in from echo cmd
	// read content of the buffer from left to right
	for (i = 0;i<8 ;i++){
		if (buffer[i]=='1'){
			gpio_set_value(GPIO_PORTB[i],1); // set the value of the port
		}
		else{
			gpio_set_value(GPIO_PORTB[i],0); // set the value of the port
		}
	}
	//Print LOG in /var/log/messages the copied content from user
	printk(KERN_INFO"driverLED[write]Buffer : %s",buffer);
	nboctets=9;
  return nb;
}



static ssize_t driverLED_read(struct file *pfile, char *poctets, size_t nb,loff_t *pos) {
	unsigned long effective_size;
	printk(KERN_INFO"driverLED[read] %d octets\n",nboctets);
	//Put GPIO in input
	//GPIO GET VALUE PORT A
	int i;
	//Read GPIO Port A (switchs)
	for (i= 7; i>=0 ;i--){
		//Set direction of the port to input
		gpio_direction_input(GPIO_PORTA[i]);
		//Get value port A bit i 
		if (gpio_get_value(GPIO_PORTA[i]) == 1){
			buffer[i] = '1';
		} else{
			buffer[i] = '0';
		}
	}
	buffer[8]='\n'; //adding backspace for printing to console
	//Print info to log /var/log/messages
	printk(KERN_INFO"driverLED[read] : nb input function :  %d\n",nb);
	printk(KERN_INFO"driverLED[read] : buffer %s\n",buffer);
	copy_to_user(poctets,buffer,nboctets); //Copy value of the buffer to the user
	nblu = nboctets; //nblu is the return value, we indicate that it has been read 8 bits
	nboctets = 0; //The flag is getting down to significate that the driver already read the value of the switch
	//print LOG
	printk(KERN_INFO"driverLED[read] nblu %d octets\n",nblu);
	
  return nblu;
}


static long driverLED_ioctl(struct file *pfile,unsigned int cmd,unsigned long arg) {
	lock_kernel();
	printk(KERN_INFO"driverLED[IOCTL] cmd : %u\n",cmd);
	unlock_kernel();
	return 0;
}

struct file_operations driverLED_fops = {
	owner: THIS_MODULE,
	open:driverLED_open,
	write:driverLED_write,
	read:driverLED_read,
	unlocked_ioctl:driverLED_ioctl,
	release:driverLED_release,
};


// initialisation
static int __init driverLED_init(void) {
	
	printk(KERN_INFO"driverLED[init] %u\n",majeur);
	register_chrdev(majeur,"driverLED", &driverLED_fops);
	int i;
	//Register all GPIO pins
	for (i=0; i<8; i++){
		gpio_request(GPIO_PORTA[i],"syfs");
		gpio_request(GPIO_PORTB[i],"syfs");
		
		gpio_export(GPIO_PORTA[i],1);
		gpio_export(GPIO_PORTB[i],1);
	}
	return 0;
}

// suppression
static void __exit driverLED_cleanup(void) {
	int i;
	//Unregister all GPIO pins
	for (i=0; i<8; i++){
		gpio_unexport(GPIO_PORTA[i]);
		gpio_unexport(GPIO_PORTB[i]);
		
		gpio_free(GPIO_PORTA[i]);
		gpio_free(GPIO_PORTB[i]);
	}
	
	unregister_chrdev(majeur,"driverLED");
	printk(KERN_INFO"driverLED[cleanup]\n");
}


// valeurs pour modinfo : description auteur licence
MODULE_DESCRIPTION("8 bits port driver with 190 major number");
MODULE_AUTHOR("Duval Clement et Care Alexis");
MODULE_LICENSE("GPL");


module_init(driverLED_init); // assignation de la fonction d'initialisation
module_exit(driverLED_cleanup); // assignation de la fonction de destruction
