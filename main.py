#!/usr/bin/python3
# Copyright (C) 2018 The Noah
import socket
from random import randint

# set some variables
buffsize = 2048
randNum = 0

# display some help
print('[+] = Good, [*] = Note, [-] = Warning, [!] Error')

# read settings.txt and put each line in an array index
with open("settings.txt") as f:
    settings = f.read().strip().split();

# set some vars
server = "irc.freenode.net"
name = "aRandomBot18f"
# set defualt vars for settings
nickname = "aRandomBot18f"
channel = "##zehra"

print("[*] Reading settings file")
for ln in settings:
    # remove spaces
    ln = ln.replace(" ","");
    # if the line starts with '#' or '//' skip it
    if ln.startswith("#") or ln.startswith("//"): continue
    # if the line stats with 'var=' then set the var to the string that comes after it
    if ln.startswith("botname="): nickname = ln.replace("botname=","");
    elif ln.startswith("channel="): channel = ln.replace("channel=","");


# encode 'text'
def encode(text):
    return text.encode("ascii")

# send 'msg' to channel 'chan' (default='channel') as a message
def send_message(msg, chan=channel):
     irc_socket.send(encode("PRIVMSG "+chan+" :"+msg+"\n"))

# send 'cmd' to channel 'chan' (default='channel')
def send_command(cmd,chan=channel):
    irc_socket.send(encode(cmd+"\n"))

# join 'chan'
def join_channel(chan):
    irc_socket.send(encode("JOIN " + chan + "\n"))

# login
def login(username=name, realname=name, hostname=name, servername=name):
    irc_socket.send(encode("USER %s %s %s %s\n" % (username, hostname, servername, realname)))
    irc_socket.send(encode("NICK " + nickname + "\n"))

print("[+] Creating socket")
# create socket
irc_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#irc_socket.settimeout(10)
#irc_socket = ssl.wrap_socket(irc_socket_raw, ssl_version=ssl.PROTOCOL_TLSv1, ciphers="ADH-AES256-SHA")
print("[*] Trying to connect")
# connect with socket
irc_socket.connect((server, 6667))
# login and join channel
login()
join_channel(channel)
# wait for 3 messages, then we're connected
for i in range(3): irc_socket.recv(buffsize)
print("[+] Connected")

# run until we quit
while True:
    # try/except used to quit with KeyboardInterrupt (Ctrl+C)
    try:
        # recieve data (up to 'buffsize' bytes)
        buffer = irc_socket.recv(buffsize)
    except KeyboardInterrupt:
        # quit main loop
        break
    # split recieved data into an array
    data = str.split(str(buffer))
    # respong to pings!
    if data[0] == "PING":
        irc_socket.send("PONG %s" % data[1])

    sender = data[0].replace(":","").replace("b'","").split("!")[0]
    if sender == botname: continue
    # used to prevent errors
    if len(data) < 2: continue
    # die if there is an error message recieved
    elif data[1] == "433" or data[0] == "b'ERROR": break
    # if a message was sent.....
    elif data[1] == "PRIVMSG":
        text = ''
        # turn data into string
        for i in range(len(data)-3):
            if i == 0: text += data[i+3][1:]
            else: text += data[i+3]
            text += ' '
        # replace some stuff
        text = text.replace("\\r\\n' ","")
        # no XD or xD allowed :D
        if text.find("XD") != -1 or text.find("xD") != -1: send_message("Stop saying that!")
        # COMMANDS
        # if someone sends '@botname:/quit' then quit
        if text.find("@"+nickname+":/quit") != -1: break
        # games!
        elif text.find("@"+nickname+":/game") != -1:
            if text.find("guess-a-number") != -1:
                randNum = randint(1,10)
                send_message("I'm thinking of a number from 1 to 10....")
        # guess a number
        elif text.find("@"+nickname+":/guess") != -1:
            if str(randNum) in text:
                send_message(sender+": Correct!")
                send_message(sender+" Wins! Let's do it again!")
                randNum = randint(1,10)
                send_message("I'm thinking of a number from 1 to 10....")
            else: send_message(sender+": Wrong :(")
        # RESPOND TO MESSAGES
        # i love you
        elif text.lower().find("i love you "+nickname.lower()) != -1:
            send_message("I love you too "+sender+"! <3")
        # if 'botname' is in the message, respond to the sender
        elif text.lower().find(nickname.lower()) != -1:
            send_message("Hello "+sender+"!")
            sender = "\033[31m"+sender+"\033[0m"
        # if the message send is not freenode, then display the message
        if text.find("freenode") == -1: print("[MSG] <"+sender+"> "+text)
    # someone joined, sasy hi
    elif data[1] == "JOIN":
        send_message("Hello "+sender+"!")

# we should quit
print("[*] Quiting")
# say goodbye
send_message("Bye! Creators: The_Noah(##thenoahbz) Zehra(##zehra)")
send_command("/part "+channel+" Quiting")
# close socket (disconnect)
irc_socket.close()
# exit program (not required but a good practice)
exit()
