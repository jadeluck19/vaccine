#!/bin/sh

cd ~/workspace/dewv/lkm
sudo dmesg -c
make
sudo insmod dewv_lkm.ko
dmesg | grep dew
