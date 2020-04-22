import sys

def write():
    print('Creating new text file')

    name = input('Enter name of text file: ')+'.txt'  # Name of text file coerced with +.txt

    file = open(name,'w')   # Trying to create a new file or open one
    file.close()

write()
