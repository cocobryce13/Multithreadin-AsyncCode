FROM gcc:latest
COPY . /app
WORKDIR /app
RUN apt update
RUN apt install -y g++ nano
RUN g++ -o Test main.cpp
CMD ["./Test"]