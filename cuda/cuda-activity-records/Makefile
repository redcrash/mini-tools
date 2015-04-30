#
# Copyright 2011-2012 NVIDIA Corporation. All rights reserved
# 
INCLUDES=-I../../include

export LD_LIBRARY_PATH := $(LD_LIBRARY_PATH):../../lib:../../lib64
LIBS= -L /usr/lib/nvidia-319 -lcuda -L /usr/lib/x86_64-linux-gnu -lcupti
OBJ = o

activity_trace: activity_trace.$(OBJ) vec.$(OBJ)
	nvcc -o $@ $^ $(LIBS)

activity_trace.$(OBJ): activity_trace.cpp
	nvcc -c $(INCLUDES) $<

vec.$(OBJ): vec.cu
	nvcc -c $(INCLUDES) $<

run: activity_trace
	./$<

clean:
	rm -f activity_trace activity_trace.$(OBJ) vec.$(OBJ)
