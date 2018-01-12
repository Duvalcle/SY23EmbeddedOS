#!/bin/bash

TTY=/dev/ttyB0

#fonction pour transformer les espace et les caractere speciaux.
urldecode() { : "${*//+/ }"; echo -e "${_//%/\\x}"; }

#header de la page html
cat << EOFHEADER
Content-type: text/html
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Message</title>
</head>
<body>
<h1 align=center >Affichage message</h1>
<hr align=center>
EOFHEADER

if [ ${REQUEST_METHOD} = "GET" ]
 then
  if [ ${#QUERY_STRING} -gt 0 ]
   then
       messageaffichage=${QUERY_STRING}
    else
      messageaffichage="Technologies Mobiles et Systèmes Embarqués"
   fi
   #debut du traitement de la chaine de caractere
   echo "<p>Message envoyé : ${messageaffichage}</p>"

   #separation des variables
   param1=${messageaffichage%%&*}
   echo "<p>param1  : ${param1}</p>"
   param2=${messageaffichage##*&}
   echo "<p>param2  : ${param2}</p>"

   #ecriture de la trame vers le driver
   echo "[00\V01\X${param2#*=}<$(urldecode ${param1#*=})>]" > $TTY
 else
   echo "<p>Erreur methode : POST</p>"
fi

echo "</body>"
echo "</html>"
