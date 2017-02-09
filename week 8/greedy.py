import sys

def main():
    if len(sys.argv) != 2 and sys.argv[1] >= 0:
        print("usage example: python greedy.py 0.41")
        return 1;
    
    change = float(sys.argv[1])
    bills = [1000, 100, 50, 20, 10, 5, 2, 1, 0.25, 0.10, 0.05, 0.01]
    notes = {}
    for bill in bills:
        notes[bill] = 0
    
    while change > 0.001:
        for bill in bills:
            if change < 0.01:
                notes[0.01] += 1
                change -= 0.01
                break
                
            elif change >= bill:
                notes[bill] += 1
                change -= bill
                break
    
    for bill in bills:
        if notes[bill] > 0:
            print(notes[bill], " of ", bill)
    return 0
    
    
    
if __name__ == "__main__":
    main()
