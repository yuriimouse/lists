# Co-authored-by: ChatGPT (gpt-4-o) <chatgpt@openai.com>
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
WORKDIR /lists

# 
COPY . .

# 
RUN make

#
CMD ["make", "test"]
