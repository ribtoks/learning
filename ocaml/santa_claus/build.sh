#!/bin/bash

rm santa_problem &> /dev/null
rm *.cmo &> /dev/null
rm *.cmi &> /dev/null
rm *.mli &> /dev/null

THREAD_PARAMS="-thread unix.cma threads.cma"
OCAML_COMMAND="ocamlc $THREAD_PARAMS"

$OCAML_COMMAND -i semaphore.ml > semaphore.mli

$OCAML_COMMAND -c semaphore.mli
$OCAML_COMMAND -c semaphore.ml

$OCAML_COMMAND semaphore.mli -i santa_problem.ml > santa_problem.mli

$OCAML_COMMAND semaphore.mli -c santa_problem.mli
$OCAML_COMMAND semaphore.mli -c santa_problem.ml

$OCAML_COMMAND -o santa_problem semaphore.cmo santa_problem.cmo

