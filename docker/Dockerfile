FROM centos:7
# Compilators and make tools
RUN yum groupinstall -y Development tools
# Install epel
RUN yum install -y epel-release
# Install CUnit of needed version
RUN yum install -y CUnit
#
RUN export LD_LIBRARY_PATH=/usr/local/custom/lib/
