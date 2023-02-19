FROM handsonsecurity/seed-ubuntu:small

RUN apt update && apt upgrade -y
RUN apt install -y build-essential gcc-multilib netcat python3
# COPY server-code/ /server-code
# WORKDIR /server-code
# RUN make && make install


COPY server    /bof/

ARG LEVEL
COPY stack-${LEVEL}  /bof/stack

WORKDIR /bof

CMD ./server
