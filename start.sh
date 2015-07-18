#Script for start the Web-Server [MyHttpd]

#首先: 检测Server是否已经启动, 如果尚未启动, 则启动之
PROGRAM=$(ps -le | grep MyHttpd | awk '{print $14}')
if [ "$PROGRAM" = "" ] ; then
	./MyHttpd
fi

#将Server启动之后, 检测其是否正在运行,
#如果其还未运行, 则打印出错信息
SERVER=$(ps -le | grep MyHttpd | awk '{print $14}')
if [ "$SERVER" != "" ] ; then
	echo "Web-Server [MyHttpd] start success ..."
else
	echo "Web-Server [MyHttpd] start error ..."
fi
