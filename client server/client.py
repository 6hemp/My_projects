import sys
import threading
import queue
import socket
import json


from server import check_cmd_flag


class Client:
    @staticmethod
    def client_thread_target(ip, port, url_queue):
        connection = socket.socket()
        connection.connect((ip, port))
        connection.recv(1)
        while True:
            try:
                url = url_queue.get(timeout=1)
                connection.send(json.dumps(url).encode())
                data = connection.recv(1024).decode()
                if not data:
                    break
                url_queue.task_done()
                print(f'{url}: {json.loads(data)}\n', end='')
            except Exception:
                pass
        connection.close()
        url_queue.task_done()

    def __init__(self, threads_count, ip, port):
        self._threads_count = threads_count
        self._url_queue = queue.Queue()
        self._ip = ip
        self._port = port
        self._threads = [
            threading.Thread(target=Client.client_thread_target,
                             args=(self._ip, self._port, self._url_queue),
                             daemon=True)
            for _ in range(self._threads_count)
        ]

    def start_threads(self):
        for thread in self._threads:
            thread.start()

    def handle(self, url):
        self._url_queue.put(url.strip())

    def join(self):
        self._url_queue.join()


def main():
    """
    Startup options:
        -t: threads count
        -i: server ip
        -p: connection port
    :return: None
    """
    urls_fn = sys.argv[-1]
    threads_count = check_cmd_flag('-t', 10)
    ip = check_cmd_flag('-i', '127.0.1.1')
    port = check_cmd_flag('-p', 15000)

    client = Client(threads_count, ip, port)
    client.start_threads()

    with open(urls_fn, 'r') as urls_file:
        for url in urls_file:
            client.handle(url)

    client.join()


if __name__ == '__main__':
    main()
