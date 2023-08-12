# Descriptions
- a simple experiment for the charDev module construction under linux based env
- use the module to print the numbers of the unique binary search tree with a given "n"

# Usage
```
make check
```

# UBST.c
- the module implementation 
- define the operations of the char Device 
```
.owner = THIS_MODULE,
    .read = ubst_read,
    .write = ubst_write,
    .open = ubst_open,
    .release = ubst_release,
    .llseek = ubst_device_llseek,
```

# client.c
- A program used to validate modules. 
- test the write and the read operations
- the results of the usbt module are stored in the out file
