#!/bin/sh

# changement du mode de fonctionnement

if [ -z "$1" ] || [ "$1" = "-h" ] || [ "$1" = "--help" ]
then
	echo "$0 [1|0|s]"
	echo "0 mode brut sans decodage"
	echo "1 mode avec decodage basic et extraction du message"
	echo "s status : mode de fonctionnement du driver"
	exit 1
fi

if [ "$1" = "s" ]
then
	cat /sys/module/driverbandeau/parameters/mode
else
 if [ "$1" = "1" ] || [ "$1" = "0" ]
  then
	iduser=`id -u`
	if [ $iduser != 0 ]
	then
		echo "il faut être en root"
	exit 1
	fi
	echo "$1" > /sys/module/driverbandeau/parameters/mode 
	exit $?
  else
   echo "erreur option $1, taper $0  pour afficher l'aide"	
   exit 1
 fi	
fi
