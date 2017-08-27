# Circuit ORAM + Pool

This library combines Pool and a pure-C conversion of [Samee's sqrtOram repo](https://github.com/samee/sqrtOram).


## To Build the Garbler & Evaluator

Run ```make``` at the root level of the repo.

## To Build the Garbler

Run ```make garb``` at the root level of the repo.

## To Build the Evaluator

Run ```make eval``` at the root level of the repo.

## To Run the Garbler

Run ```./pool_garb [port] [ORAM size]``` where ```port``` is the port the garbler and evaluator will connect on and ```ORAM size``` is the size of the ORAM in 'data units', which are 32 bit integers.

Ex: ```./pool_garb 1234 100``` -> port 1234 w/ 3200 bit ram

## To Run the Evaluator

Run ```./pool_eval [port] [ORAM size]```. The arguments should be the same as for the garbler.

## Makefile parameters
These parameters should be added with ```-D``` to the ```CFLAGS``` line of the Makefile.

- ```DEBUG_METRICS_ON```: count AND and XOR gates (default on)
- ```Set1```: Use 35k Pool size (default)
- ```Set2```: Use 35M Pool size
- ```DATA_1024```: Use 1024 bit data
- ```RECURSION_SCALE```: Set exponent for recursion cutoff.
    - Ex: ```-D RECURSION_SCALE=8``` -> 2^8 = 256 recursion cutoff


