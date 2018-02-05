import socket

server = "irc.freenode.net"
channel = "##zehra"
nickname = input("")


def encode(text):
    return text.encode("ascii")


#def ping():
#    irc_socket.send(encode("PONG :pingis\n"))


def send_message(chan, msg):
    irc_socket.send(encode("PRIVMSG " + chan + " :" + msg + "\n"))


def join_channel(chan):
    irc_socket.send(encode("JOIN " + chan + "\n"))

def login(username='aBot', realname='aBot', hostname='aBot', servername='aBot'):
    irc_socket.send(encode("USER %s %s %s %s\n" % (username, hostname, servername, realname)))
    irc_socket.send(encode("NICK " + nickname + "\n"))


irc_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
irc_socket.connect((server, 6667))
login()
join_channel(channel)

while True:
    buffer = irc_socket.recv(1024)
    msg = str.split(str(buffer))
    if msg[0] == "PING":
        irc_socket.send("PONG %s" % msg[1])
    print(msg)
