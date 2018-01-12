#!/bin/sh

# creer nouveau projet
if [ -z "$1" ]
then
  echo "ERREUR de syntaxe"
  echo "$0 nomduprojet"
  exit 1
fi

NOMPROJET=$1
echo "creation $NOMPROJET"
cp -r driver $NOMPROJET
echo "renomme drivermodele.c ${NOMPROJET}.c"
mv $NOMPROJET/drivermodele.c $NOMPROJET/$NOMPROJET.c 
echo "modifie Makefile"
sed -i -e "s/drivermodele/${NOMPROJET}/g" $NOMPROJET/Makefile
echo "modifie ${NOMPROJET}.c"
sed -i -e "s/drivermodele/${NOMPROJET}/g" $NOMPROJET/$NOMPROJET.c 
