all:
	cp ./internet-usage.sh /usr/bin/internet-usage.sh
	cp ./dbus_connection_monitor.service /etc/systemd/system/dbus_connection_monitor.service
	systemctl start dbus_connection_monitor
stop:
	systemctl stop dbus_connection_monitor
restart:
	systemctl restart dbus_connection_monitor