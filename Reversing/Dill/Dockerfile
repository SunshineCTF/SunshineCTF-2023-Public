FROM python:3.8-bookworm

RUN apt-get update && apt-get upgrade -y \
    && rm -rf /var/lib/apt/lists/*


RUN mkdir /app
WORKDIR /app
COPY requirements.txt /app/requirements.txt
RUN pip3 install -r /app/requirements.txt

COPY dill.py /app/dill.py
COPY compile.sh /app/compile.sh

ENTRYPOINT [ "/app/compile.sh"]
