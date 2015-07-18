#Script for test the server is running or not 

PROGRAM=$(ps -le | grep MyHttpd | awk '{print $14}')
if [ "$PROGRAM" != "" ] ; then
	echo "Web-Server [MyHttpd] is running..."
else
	echo "Web-Server [MyHttpd] is closed..."
fi
