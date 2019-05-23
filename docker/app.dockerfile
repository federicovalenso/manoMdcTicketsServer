FROM rabits/qt:5.12-desktop

USER root

WORKDIR /root

RUN apt-get update && \
apt-get install wget libqt5sql5-mysql -y && \
wget http://launchpadlibrarian.net/212189159/libmysqlclient18_5.6.25-0ubuntu1_amd64.deb && \
dpkg -i libmysqlclient18_5.6.25-0ubuntu1_amd64.deb && \
apt-get install -y 

RUN git clone --recursive https://github.com/federicovalenso/ClinicTicketsServer src

RUN mkdir build && \
cd build/ && \
qmake ../src/ClinicTicketsServer.pro && \
make -j4 && \
make install INSTALL_ROOT=$HOME/dist

RUN echo '[listener]\n\
port=8085\n\
minThreads=4\n\
maxThreads=100\n\
cleanupInterval=60000\n\
readTimeout=60000\n\
maxRequestSize=16000\n\
maxMultiPartSize=10000000\n\
[connetion]\n\
server_addr=mariadb\n\
user_name=root\n\
password=ctsp1qazxsw2\n\
[templates]\n\
path=templates\n\
suffix=.tpl\n\
encoding=UTF-8\n\
cacheSize=1000000\n\
cacheTime=60000\n\
[docroot]\n\
path=docroot\n\
encoding=UTF-8\n\
maxAge=60000\n\
cacheTime=60000\n\
cacheSize=1000000\n\
maxCachedFileSize=65536\n\
[sessions]\n\
expirationTime=3600000\n\
cookieName=sessionid\n\
cookiePath=/\n\
cookieComment=Identifies the user\n\
[logging]\n\
fileName=/var/log/cts.log\n\
minLevel=0\n\
bufferSize=100\n\
maxSize=1000000\n\
maxBackups=2\n\
timestampFormat=dd.MM.yyyy hh:mm:ss.zzz\n\
msgFormat={timestamp} {typeNr} {type} {thread} {msg}' > /etc/config.ini

ENTRYPOINT ["/root/build/ClinicTicketsServer"]
