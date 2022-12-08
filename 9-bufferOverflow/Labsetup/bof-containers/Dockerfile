FROM handsonsecurity/seed-ubuntu:small

# RUN apt update && apt upgrade -y
# RUN apt install build-essential
# RUN apt install gcc-multilib
# COPY server-code/ /server-code
# WORKDIR /server-code
# RUN make && make install


COPY server    /bof/

ARG LEVEL
COPY stack-${LEVEL}  /bof/stack

WORKDIR /bof

CMD ./server
