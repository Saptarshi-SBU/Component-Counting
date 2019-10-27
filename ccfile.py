import sys
import csv
import pandas as pd
import matplotlib.pyplot as plt

def GetUUID(filename):
    uid_list = []
    with open(filename) as csvfile:
        data = csv.reader(csvfile, delimiter=' ')
        for row in data:
            if row[0] not in uid_list:
                uid_list.append(row[0])
    return uid_list

def FilterByUUID(filename, uuid):
    x = []
    y = []
    with open(filename) as csvfile:
        data = csv.reader(csvfile, delimiter=' ')
        for row in data:
            if row[0] == uuid:
                x.append(int(row[1]))
                y.append(int(row[2]))
    return x, y

'''
if __name__ == "__main__":
    count = 0
    filename = sys.argv[1]
    uid_list = GetUUID(filename)
    fig, axes = plt.subplots(1, len(uid_list))
    plt.grid()
    for i in uid_list:
        x, y = FilterByUUID(filename, i)
        axes[count].scatter(x, y)
        axes[count].grid(True)
        count = count + 1
    plt.show()
'''

if __name__ == "__main__":
    #edge.dat
    filename = sys.argv[1]
    uid_list = GetUUID(filename)
    for i in uid_list:
        x, y = FilterByUUID(filename, i)
        df = pd.DataFrame(list(zip(x, y)), columns = ['EdgeX', 'EdgeY'])
        #print df
        #plt.scatter(df.EdgeX, df.EdgeY)

    #bp.dat
    filename = sys.argv[2]
    uid_list = GetUUID(filename)
    for i in uid_list:
        x, y = FilterByUUID(filename, i)
        df = pd.DataFrame(list(zip(x, y)), columns = ['BPX', 'BPY'])
        #print df
        plt.scatter(df.BPX, df.BPY)

    #result.dat
    filename = sys.argv[3]
    uid_list = GetUUID(filename)
    for i in uid_list:
        x, y = FilterByUUID(filename, i)
        df = pd.DataFrame(list(zip(x, y)), columns = ['PolyX', 'PolyY'])
        #print df
        df.plot(kind='line',x='PolyX',y='PolyY')
        #plt.scatter(df.PolyX, df.PolyY)

    plt.grid(True)
    plt.show()
