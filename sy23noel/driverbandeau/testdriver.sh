#!/bin/sh

MESSAGE="[00\V01\Xsl<test du bandeau>]"
TTY=/dev/ttyB0

echo "$MESSAGE" > $TTY
sleep 1
cat $TTY
