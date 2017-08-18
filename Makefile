# @author Darion Cassel
# Makefile for Pool - Circuit ORAM

# Change this to appropriate compiler
# for your machine (some version of gcc):
C = gcc-4.8

Path=Pool/ZH128
CommonPath=$(Path)/Common
GarbPath=$(Path)/Garb
EvalPath=$(Path)/Eval
CFLAGS= -std=c99 -w -I $(CommonPath) -msse4.2 -march=corei7-avx -o AESG.out `libgcrypt-config --cflags --libs`  -lm -O3 -D DEBUG_METRICS_ON -D MAC
CFLAGG=$(CFLAGS) -I $(GarbPath) -D POOL_GARB
CFLAGE=$(CFLAGS) -I $(EvalPath) -D POOL_EVAL

ZHFileC=$(CommonPath)/cmmc.c  $(CommonPath)/RandomMatrix.c  $(CommonPath)/galois.c  $(CommonPath)/WireLabelOperation.c  $(CommonPath)/RSEncoding.c  $(CommonPath)/RanGen.c  $(CommonPath)/PermutationOperation.c $(CommonPath)/OTExtension.c
ZHFileG=$(GarbPath)/BatchBucketProcess.c $(GarbPath)/server.c $(GarbPath)/iHash.c $(GarbPath)/GateGeneration.c $(GarbPath)/BucketProcess.c $(GarbPath)/CircuitBuild.c $(GarbPath)/BatchGateGeneration.c $(GarbPath)/InputProcess.c
ZHFileE=$(EvalPath)/BatchBucketProcess.c $(EvalPath)/client.c $(EvalPath)/iHash.c $(EvalPath)/GateGeneration.c $(EvalPath)/BucketProcess.c $(EvalPath)/CircuitBuild.c $(EvalPath)/BatchGateGeneration.c $(EvalPath)/InputProcess.c

PoolFileG= Pool/PoolCode/Garb/PoolAnd.c Pool/PoolCode/Garb/PoolAnd.h Pool/PoolCode/Garb/Functions.c Pool/PoolCode/Garb/Functions.h
PoolFileE= Pool/PoolCode/Eval/PoolAnd.c Pool/PoolCode/Eval/PoolAnd.h Pool/PoolCode/Eval/Functions.c Pool/PoolCode/Eval/Functions.h

OFILES = obliv/obliv.o obliv/obliv_gates.o oram/block.o oram/circuit_oram.o oram/copy.o oram/linear_oram.o oram/nonrecursive_oram.o oram/oram.o

.SUFFIXES: .o .c

main:	eval garb

eval: $(ZHFileC) $(ZHFileE) $(PoolFileE) $(OFILES) test/testOramAccess.c
	$(C) $(ZHFileC) $(ZHFileE) $(PoolFileE) $(OFILES) test/testOramAccess.c $(CFLAGE) -o prog_eval

garb: $(ZHFileC) $(ZHFileG) $(PoolFileG) $(OFILES) test/testOramAccess.c
	$(C) $(ZHFileC) $(ZHFileG) $(PoolFileG) $(OFILES) test/testOramAccess.c $(CFLAGG) -o prog_garb

clean:
	/bin/rm -f oram/*.o obliv/*.o test/*.o Pool/PoolCode/Eval/*.o Pool/ZH128/Common/*.o Pool/ZH128/Eval/*.o *.out

obliv.o: obliv/obliv.c obliv/obliv.h obliv/obliv_bit.h \
 obliv/obliv_gates.h obliv/../Pool/PoolCode/Garb/PoolAnd.h \
 Pool/ZH128/Eval/CircuitBuild.h Pool/ZH128/Eval/BucketProcess.h \
 Pool/ZH128/Eval/GateGeneration.h Pool/ZH128/Eval/iHash.h \
 Pool/ZH128/Eval/../Common/PermutationOperation.h \
 Pool/ZH128/Eval/../Common/aes.h Pool/ZH128/Eval/../Common/block.h \
 Pool/ZH128/Eval/../Common/RandomMatrix.h \
 Pool/ZH128/Eval/../Common/galois.h \
 Pool/ZH128/Eval/../Common/RSEncoding.h \
 Pool/ZH128/Eval/../Common/WireLabelOperation.h \
 Pool/ZH128/Eval/../Common/WireLabelOperation.h \
 Pool/ZH128/Eval/../Common/cmmc.h Pool/ZH128/Eval/../Common/rdtsc.h \
 Pool/ZH128/Eval/../Common/RanGen.h Pool/ZH128/Eval/BatchBucketProcess.h \
 Pool/ZH128/Eval/BatchGateGeneration.h
obliv_gates.o: obliv/obliv_gates.c obliv/obliv_gates.h obliv/obliv_bit.h \
 obliv/../Pool/PoolCode/Garb/PoolAnd.h Pool/ZH128/Eval/CircuitBuild.h \
 Pool/ZH128/Eval/BucketProcess.h Pool/ZH128/Eval/GateGeneration.h \
 Pool/ZH128/Eval/iHash.h Pool/ZH128/Eval/../Common/PermutationOperation.h \
 Pool/ZH128/Eval/../Common/aes.h Pool/ZH128/Eval/../Common/block.h \
 Pool/ZH128/Eval/../Common/RandomMatrix.h \
 Pool/ZH128/Eval/../Common/galois.h \
 Pool/ZH128/Eval/../Common/RSEncoding.h \
 Pool/ZH128/Eval/../Common/WireLabelOperation.h \
 Pool/ZH128/Eval/../Common/WireLabelOperation.h \
 Pool/ZH128/Eval/../Common/cmmc.h Pool/ZH128/Eval/../Common/rdtsc.h \
 Pool/ZH128/Eval/../Common/RanGen.h Pool/ZH128/Eval/BatchBucketProcess.h \
 Pool/ZH128/Eval/BatchGateGeneration.h
block.o: oram/block.c oram/block.h oram/../obliv/obliv.h \
  oram/../obliv/obliv_bit.h oram/../obliv/obliv_gates.h oram/copy.h
circuit_oram.o: oram/circuit_oram.c oram/circuit_oram.h \
  oram/../obliv/obliv.h oram/../obliv/obliv_bit.h \
  oram/../obliv/obliv_gates.h oram/block.h oram/copy.h oram/oram.h \
  oram/linear_oram.h oram/nonrecursive_oram.h
copy.o: oram/copy.c oram/../obliv/obliv.h oram/../obliv/obliv_bit.h \
  oram/../obliv/obliv_gates.h oram/copy.h
linear_oram.o: oram/linear_oram.c oram/linear_oram.h \
  oram/../obliv/obliv.h oram/../obliv/obliv_bit.h \
  oram/../obliv/obliv_gates.h oram/copy.h oram/oram.h
nonrecursive_oram.o: oram/nonrecursive_oram.c oram/nonrecursive_oram.h \
  oram/../obliv/obliv.h oram/../obliv/obliv_bit.h \
  oram/../obliv/obliv_gates.h oram/oram.h oram/copy.h oram/block.h
oram.o: oram/oram.c oram/oram.h oram/../obliv/obliv.h \
  oram/../obliv/obliv_bit.h oram/../obliv/obliv_gates.h oram/copy.h
