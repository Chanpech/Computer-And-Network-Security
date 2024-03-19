from socket import *
import sys, os

def makeReceiveFile(fileName, text):
    try:
        with open(fileName + "~received.txt", "w") as file:
            input_array = text.split("\n")
            count = 1
            while count < len(input_array):
                file.write(input_array[count] + "\n")
                count += 1
    except FileNotFoundError:
        print("FILE NOT FOUND...")
    except Exception as e:
        print("An error occured", str(e))
def findNameOfFile(file):
    input_array = file.split("\n")
    return input_array[0]


if len(sys.argv) != 3:
    print('Usage: python3 client_pop3.py <IP-address> <port>')
    sys.exit(1)
serverName = sys.argv[1]
#do something that if it is not a digit default to localhost
serverPort = int(sys.argv[2])
clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((serverName, serverPort))
while True:
    sentence = input('Input lowercase sentence:')
    if sentence == "QUIT":
        clientSocket.send(sentence.encode())
        clientSocket.close()
        break
    clientSocket.send(sentence.encode())
    modifiedSentence = clientSocket.recv(20348)
    print('From server: \n', modifiedSentence.decode())
    if "RETR" in sentence:
        fileName = findNameOfFile(modifiedSentence.decode())
        makeReceiveFile(fileName, modifiedSentence.decode())