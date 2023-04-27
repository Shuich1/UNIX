import mmap
import socket
import time

SHM_NAME = "/sharedebug"
SHM_SIZE = 1024
TCP_IP = "localhost"
TCP_PORT = 9999

if __name__ == '__main__':
    with open('/dev/shm/sharedebug', 'r+b') as shm_fd:
        shm = mmap.mmap(shm_fd.fileno(), SHM_SIZE)

        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((TCP_IP, TCP_PORT))

        while True:
            try:
                shm.seek(0)
                message = shm.read(1024).rstrip(b'\0').decode('utf-8')
                if message:
                    sock.send(message.encode("utf-8"))
                time.sleep(1)    
            except KeyboardInterrupt:
                sock.close()
                shm.close()
                shm_fd.close()
