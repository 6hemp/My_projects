import collections
import sys
import threading
import queue
import socket
import json
from urllib.request import urlopen


def check_cmd_flag(flag, default):
    if flag in sys.argv:
        flag_ind = sys.argv.index(flag) + 1
        if flag_ind < len(sys.argv):
            return int(sys.argv[flag_ind])
    return default


def parse_html(html):
    words = []
    stage = 0
    cur_str = []
    for char in html:
        if stage == 0:
            if char in ('\n', '\a', '\b', '\v', '\f', '\0', '\r', '\t', ' ', '<'):
                if len(cur_str) > 0:
                    words.append(''.join(cur_str))
                if char == '<':
                    stage = 1
                cur_str = []
            else:
                cur_str.append(char)
        elif stage == 1:
            if char == '>':
                cur_str = []
                stage = 0
    return words


def process_url(url, count):
    if not url.startswith('https://'):
        url = f'https://{url}'

    while url[-1] in ('\n', '\a', '\b', '\v', '\f', '\0', '\r', '\t', ' '):
        url = url[:-1]

    html = urlopen(url).read().decode()
    words = parse_html(html)
    c = collections.Counter(words)
    most_common_dict = {elem[0]: elem[1] for elem in c.most_common(count)}
    return most_common_dict


class Server:
    @staticmethod
    def worker_target(request_queue, common_words_count, url_processed_callback):
        while True:
            try:
                client, url = request_queue.get(timeout=1)

                success = True
                try:
                    data = process_url(url, common_words_count)
                except Exception as ex:
                    data = str(ex)
                    success = False

                client.send(json.dumps(data).encode())
                url_processed_callback(success)
                request_queue.task_done()
            except Exception:
                pass

    @staticmethod
    def handler_target(client, request_queue):
        while True:
            data = client.recv(1024).decode()
            if not data:
                break
            request_queue.put((client, json.loads(data)))
        client.close()

    def __init__(self, workers_count, common_words_count, ip, port):
        self._proc_url_count = 0
        self._fail_url_count = 0
        self._workers_count = workers_count
        self._common_words_count = common_words_count
        self._request_queue = queue.Queue()
        self._workers = [
            threading.Thread(target=Server.worker_target,
                             args=(self._request_queue,
                                   self._common_words_count,
                                   lambda success: (self._inc_counter(success), self.print_counter())),
                             daemon=True)
            for _ in range(workers_count)
        ]

        self._sock = socket.socket()
        self._sock.bind((ip, port))
        self._sock.listen()

        self._clients_handlers = []

    def make_handler(self, client):
        client_handler = threading.Thread(target=Server.handler_target,
                                          args=(client, self._request_queue),
                                          daemon=True)
        self._clients_handlers.append(client_handler)
        client_handler.start()

    def print_counter(self):
        print(f'Processed urls: {self._proc_url_count}\t', end='')
        print(f'Failed urls: {self._fail_url_count}\n', end='')

    def start_workers(self):
        for thread in self._workers:
            thread.start()

    def accept(self):
        client, _ = self._sock.accept()
        client.send(b'1')
        return client

    def join_clients_handlers(self):
        for thread in self._clients_handlers:
            thread.join()

    def join_workers(self):
        self._request_queue.join()

    def join(self):
        self.join_workers()
        self.join_clients_handlers()

    def close(self):
        self._sock.close()

    def _inc_counter(self, success=True):
        with threading.Lock():
            if success:
                self._proc_url_count += 1
            else:
                self._fail_url_count += 1


def main():
    """
    Ctrl+C = soft close
    double Ctrl+C = force close
    Startup options:
        -w: workers count
        -k: common words count
        -p: host port
    """
    workers_count = check_cmd_flag('-w', 10)
    common_words_count = check_cmd_flag('-k', 7)
    ip = socket.gethostbyname(socket.gethostname())
    port = check_cmd_flag('-p', 15000)

    server = Server(workers_count, common_words_count, ip, port)
    server.start_workers()

    try:
        while True:
            client = server.accept()
            server.make_handler(client)
    except KeyboardInterrupt:
        try:
            server.join()
            server.close()
        except KeyboardInterrupt:
            server.close()


if __name__ == '__main__':
    main()
