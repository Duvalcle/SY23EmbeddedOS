#!/bin/bash

TTY=/dev/ttyB0
#TTY=/dev/ttyS1

urldecode() { : "${*//+/ }"; echo -e "${_//%/\\x}"; }

# programme à compléter
# insérer la aprtie de gestion du bandeau connecté à TTY
# en simulation le message envoyé sera accessible avec
# cat /dev/ttyB0

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
   echo "<p>Message envoyé : ${messageaffichage}</p>"
   param1=${messageaffichage%%&*}
   echo "<p>param1  : ${param1}</p>"
   param2=${messageaffichage##*&}
   echo "<p>param2  : ${param2}</p>"
   echo "[00\V01\X${param2#*=}<$(urldecode ${param1#*=})>]" > $TTY
   #echo "<p>$(cat $TTY)</p>"
 else
   echo "<p>Erreur methode : POST</p>"
fi

echo "</body>"
echo "</html>"
