# Descriptions
- An experiment for the charDev module construction under linux based env.
- Use the module to calculate the numbers of the unique binary search tree with a given "n".

# Usage
- Construct and load the module into the system.
```
make check
```
- Remove files other than the source code.
```
make clean
```

# UBST.c
- The module implementation by define the operations of the char device.
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
- Test the read、write and llseek operations of char device. 
- Put the results of the usbt module in the out file.
