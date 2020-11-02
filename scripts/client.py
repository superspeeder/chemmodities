import sys
import requests
import socket

import zipfile
import os
import shutil

POST_S = "POST"
GET_S = "GET"

# IP = "73.70.139.207"
IP = "127.0.0.1"

def zipdir(path, ziph):
    # ziph is zipfile handle
    for root, dirs, files in os.walk(path):
        for file in files:
            ziph.write(os.path.join(root, file))

class PostSideClient:
    def __init__(self, server_ip, server_port, path):
        zipf = zipfile.ZipFile("game.zip", 'w', zipfile.ZIP_DEFLATED)
        with zipf:
            zipdir(path, zipf)
        try:
            self.sendFile(server_ip, server_port, "game.zip")
        except:
            pass

        print("Sent")

    def sendFile(self,ip,port,path):
        with open(path,'rb') as f:
            requests.put(f"http://{ip}:{port}",data=f)

class GetSideClient:
    def __init__(self, server_ip, server_port, http_port, destination, source_filename):
        self.http_port = http_port
        self.server_ip = server_ip
        self.server_port = server_port
        self.destination = destination
        self.source_filename = source_filename

        self.recv_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.recv_sock.connect((server_ip,server_port))

        self.run()

        self.recv_sock.close()

    def run(self):
        while True:
            data = self.recv_sock.recv(4)

            if data.startswith(bytes([0xF,0xF])):
                r = requests.get(f"http://{self.server_ip}:{self.http_port}/game.zip", stream=True)
                with open("game.zip", 'wb+') as f:
                    r.raw.decode_content = True
                    shutil.copyfileobj(r.raw, f)

                with zipfile.ZipFile("game.zip") as zf:
                    zf.extractall()


                print("saved")


if __name__ == "__main__":
    side = sys.argv[1].upper()

    if side == POST_S:
        PostSideClient(IP, 40312, "output/")
    else:
        GetSideClient(IP, 40311, 40312, "game.zip", "game.zip")
