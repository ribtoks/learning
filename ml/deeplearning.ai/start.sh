#!/bin/bash

# docker build -t tklearnml .

docker run -p 8888:8888 -p 6006:6006 -v $(pwd)/notebooks:/notebooks tklearnml
