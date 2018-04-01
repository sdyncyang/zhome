#!/bin/bash

for ((i=0; i<=9; i=i+1))
do
	url="http://app1.showapi.com/weather/icon/day/0"${i}".png"
	wget $url
done
