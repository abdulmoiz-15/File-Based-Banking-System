# C++ File Based Banking System
A C++ file-based banking system built with Object-Oriented Programming, featuring secure authentication, account management, transactions, loan processing, and persistent data storage.

# Setup Instructions

This program stores data in `.txt` files at hardcoded paths. Follow these steps before running it.

## 1. Create the Folder

Create a folder named `Files` on any local disk, e.g.:

```
E:/Files/
```

## 2. Create These Files Inside It

```
count.txt
loans.txt
1.txt
2.txt
3.txt
4.txt
5.txt
6.txt
7.txt
8.txt
9.txt
10.txt
```

All files should be **empty**, except `count.txt`, which should contain:

```
0
```

## 3. Change the Path in the Code (if not using `E:`)

If you don't have an `E:` drive, open the code and change the path in these two places to match your system:

```cpp
const string countFile = "E:/Files/count.txt";

string files[10] = {
    "E:/Files/1.txt", "E:/Files/2.txt", "E:/Files/3.txt",
    "E:/Files/4.txt", "E:/Files/5.txt", "E:/Files/6.txt",
    "E:/Files/7.txt", "E:/Files/8.txt", "E:/Files/9.txt",
    "E:/Files/10.txt"
};
```

Replace `E:/Files/` with your own path, for example:

```cpp
const string countFile = "C:/Files/count.txt";

string files[10] = {
    "C:/Files/1.txt", "C:/Files/2.txt", "C:/Files/3.txt",
    "C:/Files/4.txt", "C:/Files/5.txt", "C:/Files/6.txt",
    "C:/Files/7.txt", "C:/Files/8.txt", "C:/Files/9.txt",
    "C:/Files/10.txt"
};
```

Make sure the folder path in the code matches the folder you created in Step 1.

## 4. Compile & Run

## Login as Admin

```
Username: admin
PIN: 1111
```
