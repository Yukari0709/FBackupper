# FBackupper
A lightweight file backup and recovery software. Supported features include: 
- Backup＆Restore;
- Special files for Unix filesystem(soft＆hard link, pipe...);
- Metadata for Unix filesystem;
- User-defined file filtering;
- File Pack, Compress and Encrypt.

Originally developed by Shao Jiang jshao404@gmail.com, Junjie Wang and Leedfree.

# Dependency 
- **OpenSSL:** = 1.1.1.
```bash
sudo apt-get install libssl-dev
```
- **CMake:** >= 3.12.
- **gcc＆g++:** >= 9.4.0.

# Build
> We strongly recommend building under Ubuntu 20.04 version.

mkdir build && cd build  
cmake ..  
make  

# Usage
``` bash
FBackupper [Options] [Paras] ...
```
Use empty parameters: ./Fbackupper for more detail.
