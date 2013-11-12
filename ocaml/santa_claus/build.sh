#!/bin/bash

rm santa_problem
rm *.cmo
rm *.cmi

ocamlc -thread -i unix.cma threads.cma semaphore.ml > semaphore.mli

ocamlc -thread unix.cma threads.cma -c semaphore.mli
ocamlc -thread unix.cma threads.cma -c semaphore.ml

ocamlc -thread threads.cma unix.cma semaphore.mli -i santa_problem.ml > santa_problem.mli

ocamlc -thread threads.cma unix.cma semaphore.mli -c santa_problem.mli
ocamlc -thread threads.cma unix.cma semaphore.mli -c santa_problem.ml

ocamlc -thread unix.cma threads.cma -o santa_problem semaphore.cmo santa_problem.cmo

