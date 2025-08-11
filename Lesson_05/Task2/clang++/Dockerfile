FROM ubuntu:16.04
COPY . /app
WORKDIR /app
RUN apt update
RUN apt install -y clang nano
RUN clang++ -std=c++14 -pthread -o Test main.cpp
CMD ["./Test"]