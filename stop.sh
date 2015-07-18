#Script for shutdown the Web-Server [MyHttpd]

#首先: 检测Server是否正在运行, 如果正在运行, 则关闭之
PROGRAM=$(ps -le | grep MyHttpd | awk '{print $14}')
if [ "$PROGRAM" != "" ] ; then
	MyHttpd=$(ps -le | grep MyHttpd | awk '{print $4}')
    kill -9 "$MyHttpd"
fi

#将Server关闭之后, 检测其是否正在运行,
#如果其还在运行, 则打印出错信息
SERVER=$(ps -le | grep MyHttpd | awk '{print $14}')
if [ "$SERVER" = "" ] ; then
	echo "Web-Server [MyHttpd] close success..."
else
	echo "Web-Server [MyHttpd] close error..."
fi
