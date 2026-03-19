#!/bin/bash

set -e

IMAGE_NAME="ulimit_test_image"

cat <<EOF | docker build -q -t $IMAGE_NAME -f - .
FROM ubuntu:22.04
RUN apt-get update && apt-get install -y gcc && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY task3-14.c .
RUN gcc task3-14.c -o task3-14
ENTRYPOINT ["/app/task3-14"]
EOF

printf "\n[ Експеримент 1: nofile ]\n"

printf "Стандартні налаштування:\n"
docker run --rm $IMAGE_NAME nofile || true

printf "\nОбмеження nofile=20:20:\n"
docker run --rm --ulimit nofile=20:20 $IMAGE_NAME nofile || true

printf "\n[ Експеримент 2: fsize ]\n"

printf "Стандартні налаштування:\n"
docker run --rm $IMAGE_NAME fsize || true

printf "\nОбмеження fsize=10485760:10485760 (10 МБ):\n"
docker run --rm --ulimit fsize=10485760:10485760 $IMAGE_NAME fsize || true
