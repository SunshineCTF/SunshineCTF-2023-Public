FROM ubuntu:23.10
WORKDIR /opt/chal

RUN apt-get update && apt-get install -y aubio-tools jq curl coreutils gawk sed

COPY . /opt/chal

RUN chmod a+x /opt/chal/wav_file_solver.sh && chmod a+rwx /opt/chal

WORKDIR /opt/chal
ENTRYPOINT ["/opt/chal/wav_file_solver.sh"]
