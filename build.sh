#!/bin/bash

gcc rpigpio.c -shared -fpic -o rpigpio.so `pkg-config lua5.1 --libs`
