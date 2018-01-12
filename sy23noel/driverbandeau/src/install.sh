#!/bin/sh

# installation driver bandeau
iduser=`id -u`

if [ $iduser != 0 ]
then
 echo "il faut être en root"
 exit 1
fi

echo "installation"
echo "copie 10-ttybandeau.rules"
cp 10-ttybandeau.rules /etc/udev/rules.d
echo "credémarre udev"
/etc/init.d/udev restart
if [ -f driverbandeau.ko ] 
then
 echo "installe le driver"
 insmod ./driverbandeau.ko
else
 echo "driverbandeau.ko non installé"
 echo "le compiler en utilisateur standard si non présent"
fi
