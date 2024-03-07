FROM python:3.11-alpine
WORKDIR /opt/chal

RUN apk add --update \
    && apk add \
        python3-dev \
        gcc \
        musl-dev \
        g++ \
        bash

COPY requirements.txt /opt/chal/

RUN pip install -r requirements.txt --no-cache-dir

COPY . /opt/chal

# hadolint ignore=SC2086
# note: this Dockerfile is based on https://github.com/SunshineCTF/SunshineCTF-2017-Public/blob/master/Pwn/400-Notetorious/Dockerfile

# Create unprivileged unprivileged user. lol.
RUN addgroup -S unprivileged && \
    adduser -S unprivileged \
        -G unprivileged \
        -g "" \
        -s "/bin/bash" && \
    mkdir /opt/chal/flask_session && \
    chown root:unprivileged /opt/chal/flask_session && \
    chmod 070 /opt/chal/flask_session && \
    chmod u+x /opt/chal/wavfile_generator/build.sh

RUN /opt/chal/wavfile_generator/build.sh
WORKDIR /opt/chal

RUN chmod +x /opt/chal/docker-entrypoint.sh

USER unprivileged
EXPOSE 5000
ENTRYPOINT ["/opt/chal/docker-entrypoint.sh"]
