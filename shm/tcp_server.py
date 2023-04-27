import socket

TCP_IP = "localhost"
TCP_PORT = 9999


if __name__ == '__main__':
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind((TCP_IP, TCP_PORT))
    sock.listen(1)
    conn, addr = sock.accept()

    while True:
        try:
            data = conn.recv(1024)
            if not data:
                break
            message = data.decode("utf-8")
            print("Received message: {}".format(message))
        except KeyboardInterrupt:
            conn.close()
            sock.close()
