#!/bin/sh

affiche_aide() {
	echo "$0 [options]"
	echo " -h | --help    : affiche l'aide"
	echo " -r | --remove  : rmmod driverbandeau"
	echo " -d | --decode  : decode les trames du bandeau"
}


# installation driver bandeau
iduser=`id -u`

if [ $iduser != 0 ]
then
 echo "il faut être en root"
 exit 1
fi

MODE=2
case $1 in
		-h | --help)	affiche_aide
						exit 0
						;;
		-r | --rmmod)	rmmod driverbandeau
						exit $?
						;;
		-d | --decode)	MODE=1
						;;		
		-b | --brut)	MODE=0
						;;								
esac


if [ $MODE = 2 ]
then
	lsmod | grep driverbandeau > /dev/null
	if [ $? = 0 ]
	then
		echo "driverbandeau installé"
		echo "$0 -r pour le désinstaller"
	else
		echo "driverbandeau non installé"
		echo "$0 -d | -b  pour l'installer"
		echo "-d avec extraction du message de la chaîne"
		echo "-b chaîne complète transmise sans extraction du message"
	fi
else
	if [ -f driverbandeau.ko ] 
	then
		echo "installe le driver mode $MODE"
		insmod ./driverbandeau.ko mode=$MODE
		exit $?
	else
		echo "driverbandeau.ko non installé"
		echo "le compiler en utilisateur standard si non présent"
		exit 1
	fi
fi
