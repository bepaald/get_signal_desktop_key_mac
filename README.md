# get_signal_desktop_key_mac
Since Signal Desktop 7.17, the cipher key used to read the database is encrypted. This program attempts to decrypt the key. For this it tries to read a password from the your keychain.

# Requirements

Note this program is macOS only. For Windows, decrypting the keys is already part of [signalbackup-tools](https://github.com/bepaald/signalbackup-tools).

This program depends on
- A c++ compiler, supporting c++17
- cmake (`brew install cmake`)
- openssl (`brew install openssl`)

# Compile

Simple download & compile:
```
$ git clone https://github.com/bepaald/get_signal_desktop_key_mac
$ cd get_signal_desktop_key_mac
$ cmake -B build
$ cmake --build build
```
Change/add any options if you know better.

# Run

Simply run the binary from the command line. Assuming it is located in the `build` directory passed to the `cmake` commands listed above:

```
$ ./build/get_signal_desktop_key_mac
```
The program expects to find the Signal Desktop config file in `~/Library/Application Support/Signal/config.json`. If your config file is at a different location (or you're on the Beta), you can supply it as an argument to the program. For example:
```
$ ./build/get_signal_desktop_key ~/Library/Application Support/Signal Beta/json.config
```

If the program works, I would love to hear about it. Let me know what macOS version you are using, and any warnings, errors, or any unexpected output you encounter while compiling or running this program by adding a message to the first issue [here](https://github.com/bepaald/get_signal_desktop_key_mac/issues/1). Thanks!

If the program consistently fails, try adding `-v` to the command line for more verbose output and opening an issue.

# Future plans

It is planned to incorparate this functionality into [signalbackup-tools](https://github.com/bepaald/signalbackup-tools) in the future. However, for now
- I am very short on time
- I am very unsure currently if it works at all for most people.


