import sys

if len(sys.argv) != 3:
    print("Usage: python script.py <arg1> <argv2>")
    sys.exit(1)

arg1 = sys.argv[1]
arg2 = sys.argv[2]

print("Argument 1:", arg1)
print("Argument 2:", arg2)