import random

keyType = "uint32_t"
valType = "uint32_t"

counts = []

for i in range(10, 2000, 100):
    counts.append(i)

maxCount = counts[-1]

random.seed()

keys = []
vals = []

for i in range(maxCount):
    while True:
        randKey = random.randint(1, 100000)
        if not randKey in keys:
            keys.append(randKey)
            break
    while True:
        randVal = random.randint(1, 100000)
        if not randVal in vals:
            vals.append(randVal)
            break

print("{0} keys[] = {{".format(keyType))
for i in range(maxCount):
    if i > 0:
        print(", ", end="")
    print(keys[i], end="")
print("};")

print("{0} vals[] = {{".format(valType))
for i in range(maxCount):
    if i > 0:
        print(", ", end="")
    print(vals[i], end="")
print("};")


def genSwitch(itemCount):
    print("{0} benchSwitch{1}({2} key){{".format(valType, itemCount, keyType))
    print("switch(key) {")
    for i in range(itemCount):
        print("case {0}: return {1};".format(keys[i], vals[i]))
    print("default: return 0;\n}\n}")


for count in counts:
    genSwitch(count)

print("typedef {0} (*SwitchFunction)({1} key);".format(valType, keyType))
print("SwitchFunction switchFuns[] = {")
for count in counts:
    if count != counts[0]:
        print(", ", end="")
    print("benchSwitch{0}".format(count), end="")
print("};")
print("const size_t funCount = {0};".format(len(counts)))

print("size_t counts[] = {")
for count in counts:
    if count != counts[0]:
        print(", ", end="")
    print(count, end="")
print("};")
