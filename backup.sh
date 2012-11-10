VERSION=`cat include/version.h | awk -F\" '{print $2}'`
echo $VERSION
tar -cvjf $VERSION.tar.bz2 qc/* 
echo "compressed tar ball is ready"
