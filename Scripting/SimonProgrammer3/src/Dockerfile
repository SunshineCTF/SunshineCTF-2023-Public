FROM python:3.11-alpine AS original_layer
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
    chmod u+x /opt/chal/wavfile_generator/build.sh && \
    /opt/chal/wavfile_generator/build.sh && \
    chmod a+r /opt/chal/freq_map.txt

RUN apk del \
        python3-dev \
        gcc \
        musl-dev \
        g++ \
        bash && rm /var/cache/apk/*

FROM python:3.11-alpine AS distro_chal
COPY --from=original_layer /etc /etc
COPY --from=original_layer /opt /opt
COPY --from=original_layer /lib /lib
COPY --from=original_layer /usr /usr

RUN chown root:unprivileged /opt/chal/flask_session && \
    chmod 070 /opt/chal/flask_session && \
    chmod a+r /opt/chal/freq_map.txt && apk add \
        --no-cache bash

WORKDIR /opt/chal

RUN chmod a+x /opt/chal/docker-entrypoint.sh && chmod a+r -R /opt/chal && chmod a+rx /opt/chal/index.py

USER unprivileged
EXPOSE 5002
ENTRYPOINT ["/opt/chal/docker-entrypoint.sh"]
