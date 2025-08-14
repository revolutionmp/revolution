FROM debian:12.11-slim
RUN dpkg --add-architecture i386
RUN apt-get update && apt-get install -y git wget curl ca-certificates libc6:i386 lib32stdc++6 && rm -rf /var/lib/apt/lists/*
RUN curl https://raw.githubusercontent.com/Southclaws/sampctl/master/install-deb.sh | sed "s/sudo //g" | sh
WORKDIR /evolve-testing
COPY . .
RUN chmod +x ./compile.sh && ./compile.sh
EXPOSE 7777/tcp
EXPOSE 7777/udp
EXPOSE 8888/udp
CMD ["sampctl", "run"]