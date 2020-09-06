sysctl -w net.ipv4.ip_forward=1
iptables -I INPUT --source 10.0.0.0/24 -j ACCEPT
iptables -t nat -I POSTROUTING --out-interface eth0 -j MASQUERADE
iptables -I FORWARD --in-interface eth0 --out-interface tap0 -j ACCEPT
iptables -I FORWARD --in-interface tap0 --out-interface eth0 -j ACCEPT