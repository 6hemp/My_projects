import threading
import unittest
import socket

import client


def run_fake_server():
    server_sock = socket.socket()
    server_sock.bind(('127.0.0.1', 7777))
    server_sock.listen(0)
    server_sock.accept()
    server_sock.close()


class TestClient(unittest.TestCase):
    def test_client_connect_and_disconnect_to_server(self):
        serv_thread = threading.Thread(target=run_fake_server, args=tuple())
        serv_thread.start()

        clt = client.Client(1, '127.0.0.1', 7777)
        clt.start_threads()

        clt.join()


if __name__ == '__main__':
    unittest.main()
