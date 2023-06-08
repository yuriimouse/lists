FROM centos:7
# Install epel
RUN yum install -y epel-release
# Add sclo utility 
RUN yum install -y centos-release-scl-rh
# Compilators and make tools
RUN yum groupinstall -y Development tools
# Install CUnit of needed version
RUN yum install -y CUnit-devel
#
RUN export LD_LIBRARY_PATH=/usr/local/custom/lib/
