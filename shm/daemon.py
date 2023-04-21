import mmap
import time


if __name__ == '__main__':
    with open('/dev/shm/sharedebug', 'r+b') as f:
        mm = mmap.mmap(f.fileno(), 1024)

        last_msg = None

        while True:
            mm.seek(0)
            msg = mm.read(1024).rstrip(b'\0').decode('utf-8')

            if msg != last_msg:
                print(f'Received message: {msg}')
                last_msg = msg

            time.sleep(0.1)
