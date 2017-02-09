import sys

def main():
    if(len(sys.argv) != 2):
        print("Please provide a single input string!")
        return 1
    
    userInput = input("plaintext: ")
    
    k = int(sys.argv[1])
    
    for letter in userInput:
        letterNum = ord(letter)
        if letterNum >= 65 and letterNum <= 65 + 26:
            print(chr(((letterNum - 65 + k) % 26) + 65), end = '')
        elif letterNum >= 97 and letterNum <= 97 + 26:
            print(chr(((letterNum - 97 + k) % 26) + 97), end = '')
        else:
            print(letter)
    print("")
    return 0
    
if __name__ == "__main__":
    main()
