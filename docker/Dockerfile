FROM ubuntu:latest

# 
ENV DEBIAN_FRONTEND=noninteractive

# 
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    make \
    git \
    valgrind \
    && rm -rf /var/lib/apt/lists/*

# 
WORKDIR /app

# 
COPY . .

# 
RUN make

# Запуск тестов по умолчанию при запуске контейнера
CMD ["make", "test"]
