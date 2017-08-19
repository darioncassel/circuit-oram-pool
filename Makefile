# @author Darion Cassel
# Makefile for Pool - Circuit ORAM

# Change this to appropriate compiler
# for your machine (some version of gcc):
C = gcc-4.8 -std=c99 

Path=Pool/ZH128
CommonPath=$(Path)/Common
GarbPath=$(Path)/Garb
EvalPath=$(Path)/Eval
CFLAGS= -w -g -I $(CommonPath) -msse4.2 -march=corei7-avx -o AESG.out `libgcrypt-config --cflags --libs`  -lm -O3 -D DEBUG_METRICS_ON -D MAC
CFLAGG=$(CFLAGS) -I $(GarbPath) -D POOL_GARB
CFLAGE=$(CFLAGS) -I $(EvalPath) -D POOL_EVAL

ZHFileC=$(CommonPath)/cmmc.c  $(CommonPath)/RandomMatrix.c  $(CommonPath)/galois.c  $(CommonPath)/WireLabelOperation.c  $(CommonPath)/RSEncoding.c  $(CommonPath)/RanGen.c  $(CommonPath)/PermutationOperation.c $(CommonPath)/OTExtension.c
ZHFileG=$(GarbPath)/BatchBucketProcess.c $(GarbPath)/server.c $(GarbPath)/iHash.c $(GarbPath)/GateGeneration.c $(GarbPath)/BucketProcess.c $(GarbPath)/CircuitBuild.c $(GarbPath)/BatchGateGeneration.c $(GarbPath)/InputProcess.c
ZHFileE=$(EvalPath)/BatchBucketProcess.c $(EvalPath)/client.c $(EvalPath)/iHash.c $(EvalPath)/GateGeneration.c $(EvalPath)/BucketProcess.c $(EvalPath)/CircuitBuild.c $(EvalPath)/BatchGateGeneration.c $(EvalPath)/InputProcess.c

PoolFileG= Pool/PoolCode/Garb/PoolAnd.c Pool/PoolCode/Garb/PoolAnd.h Pool/PoolCode/Garb/Functions.c Pool/PoolCode/Garb/Functions.h
PoolFileE= Pool/PoolCode/Eval/PoolAnd.c Pool/PoolCode/Eval/PoolAnd.h Pool/PoolCode/Eval/Functions.c Pool/PoolCode/Eval/Functions.h

OblivFiles = obliv/obliv.c obliv/obliv_gates.c oram/block.c oram/circuit_oram.c oram/copy.c oram/linear_oram.c oram/nonrecursive_oram.c oram/oram.c

main:	eval garb

eval: $(ZHFileC) $(ZHFileE) $(PoolFileE) $(OblivFiles) test/testOramAccess.c
	$(C) $(ZHFileC) $(ZHFileE) $(PoolFileE) $(OblivFiles) test/testOramAccess.c $(CFLAGE) -o pool_eval

garb: $(ZHFileC) $(ZHFileG) $(PoolFileG) $(OblivFiles) test/testOramAccess.c
	$(C) $(ZHFileC) $(ZHFileG) $(PoolFileG) $(OblivFiles) test/testOramAccess.c $(CFLAGG) -o pool_garb

clean:
	/bin/rm -f oram/*.o obliv/*.o test/*.o Pool/PoolCode/Eval/*.o Pool/ZH128/Common/*.o Pool/ZH128/Eval/*.o *.out
