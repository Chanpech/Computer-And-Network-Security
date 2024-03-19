#Should run forever
import sys, os
from socket import *

filePath = ["sample-email-1.txt", "sample-email-2.txt", "sample-email-3.txt"]
fileExistence = [1, 1 ,1]
numOfFilesAvaliable = 3

def findFile(a):
    try:
        with open(filePath[a], "r") as file:
            fileContent = file.read()
            return fileContent
    except FileNotFoundError:
        print("FILE NOT FOUND...")
    except Exception as e:
        print("An error occured", str(e))

def listEmail():
    try:
        numOfFiles = len(filePath)
        count = 0
        listToSend = ""
        while count < numOfFiles:
            if(fileExistence[count] == 0):
                count += 1
                continue
            fileSize =os.path.getsize(filePath[count])
            listToSend = listToSend + str(count) + " " + str(fileSize) + " bytes\n"
            count += 1
        return listToSend
    except FileNotFoundError:
        print("FILE NOT FOUND...")
    except Exception as e:
        print("An error occured", str(e))

def deleFile(i):
    try:
        global fileExistence
        fileExistence[i] = 0
        global numOfFilesAvaliable
        numOfFilesAvaliable = numOfFilesAvaliable - 1
        os.remove(filePath[i])
        print("The file at index " + str(i) +" has been deleted.")
    except FileNotFoundError:
        print("FILE NOT FOUND...")
    except Exception as e:
        print("An error occured", str(e))

def findTop(fileIndex, numOfLines):
    try:
        foundTop = ""
        with open(filePath[fileIndex], "r") as file:
            count = 0
            currentLine = file.readline()
            foundTop = foundTop + currentLine
            while currentLine:
                if len(currentLine.strip()) <=0: 
                    break
                currentLine = file.readline()
                foundTop = foundTop + currentLine
            while count < numOfLines:
                if count >= numOfLines:
                   break
                currentLine = file.readline()
                foundTop = foundTop + currentLine
                count += 1
        return foundTop
    except FileNotFoundError:
        print("FILE NOT FOUND...")
    except Exception as e:
        print("An error occured", str(e))

def findFileName(i):
    name = filePath[i].split(".txt")
    return name[0]

if len(sys.argv) != 2:
    print('Usage: python3 server_pop3.py <port>')
    sys.exit(1)
serverPort = int(sys.argv[1])
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind(('', serverPort))
serverSocket.listen(1)
print('Server ready to receive...')
connectionSocket, addr = serverSocket.accept()
terminated = False
while True:
    if(terminated):
        print('Server ready to receive...')
        connectionSocket, addr = serverSocket.accept()
    print("+OK POP3 server ready...")
    sentence = connectionSocket.recv(2048).decode()
    capitalizedSentence = sentence.upper()
    input_array = capitalizedSentence.split()
    
    if input_array[0] == "STAT":
        print("+OK ")
        numOfFileStored = str(numOfFilesAvaliable) + " messsages stored"
        connectionSocket.send(numOfFileStored.encode())

    elif input_array[0] == "LIST":
        print("+OK " + str(numOfFilesAvaliable) + " messages:")
        foundList = listEmail()
        connectionSocket.send(foundList.encode())

    elif input_array[0] == "RETR":
        if(len(input_array) != 2):
            print("-ERR RETR <X>")
            connectionSocket.send("ERROR: RETR <X>".encode())
        elif int(input_array[1]) >= len(filePath):
            print("-ERR RETR <X>")
            connectionSocket.send("ERROR: RETR <X> do not exists".encode())
        else:
            print("+OK " + str(input_array[1]))
            retrievedFile = findFile(int(input_array[1]))
            fileName = findFileName(int(input_array[1]))
            print("Client has successfully received the email, and stored it locally")     
            connectionSocket.send((fileName + "\n"+ retrievedFile).encode())

    elif  input_array[0] == "DELE":
        if(len(input_array) != 2):
            print("-ERR DELE <X>")
            connectionSocket.send("ERROR: DELE <X>".encode())
        elif(int(input_array[1]) >= len(filePath)):
            print("-ERR DELE <X>")
            connectionSocket.send("ERROR: DELE <X> do not exist".encode())
        else:
            print("+OK Delete Email " + str(input_array[1]))
            deleFile(int(input_array[1]))
            connectionSocket.send(capitalizedSentence.encode())
    
    elif input_array[0] == "TOP":
        if(len(input_array) != 3):
            print("-ERR TOP <X> <n>")
            connectionSocket.send("ERROR: TOP <X> <n>".encode())
        elif(int(input_array[1]) >= 3):
            print("-ERR TOP <X> <n>")
            connectionSocket.send("ERROR: TOP <X> <n> out of bound".encode())
        else:
            print("+OK sending header and the first n")
            foundTop = findTop(int(input_array[1]), int(input_array[2]))
            connectionSocket.send(foundTop.encode())
    
    elif input_array[0] == "QUIT":
        print("+OK Terminating the conversation...")
        terminated = True
    else:
        print("-ERR")
        connectionSocket.send("ERROR: Invalid Command".encode())
connectionSocket.close()
