def main():
    mario(7);
    
def mario(n):
    for i in range(1, n + 1):
        print( (n - i) * " ", i * "#", " ", i * "#")
    
    
if __name__ == "__main__":
    main()
