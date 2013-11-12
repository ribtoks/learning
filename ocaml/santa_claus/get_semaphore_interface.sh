#!/bin/bash
ocamlc -thread -i unix.cma threads.cma semaphore.ml > semaphore.mli
