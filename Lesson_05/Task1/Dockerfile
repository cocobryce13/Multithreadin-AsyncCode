FROM ubuntu:16.04
COPY . /app
WORKDIR /app
RUN apt update
RUN apt install -y g++ nano
RUN g++ -o Test task1.cpp
CMD ["./Test"]