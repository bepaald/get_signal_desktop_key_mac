/*
  Copyright (C) 2024  Selwin van Dijk

  This file is part of get_signal_desktop_key_mac.

  get_signal_desktop_key_mac is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  get_signal_desktop_key_mac is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with get_signal_desktop_key_mac.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <Security/Security.h>

#include "main.h"
#include <iostream>

/*

  security dump-keychain:

keychain: "/Users/bastimmer/Library/Keychains/login.keychain-db"
version: 512
class: "genp"
attributes:
    0x00000007 <blob>="Signal Safe Storage"
    0x00000008 <blob>=<NULL>
    "acct"<blob>="Signal Key"
    "cdat"<timedate>=0x32303234303831353134333230395A00  "20240815143209Z\000"
    "crtr"<uint32>="aapl"
    "cusi"<sint32>=<NULL>
    "desc"<blob>=<NULL>
    "gena"<blob>=<NULL>
    "icmt"<blob>=<NULL>
    "invi"<sint32>=<NULL>
    "mdat"<timedate>=0x32303234303831353134333230395A00  "20240815143209Z\000"
    "nega"<sint32>=<NULL>
    "prot"<blob>=<NULL>
    "scrp"<sint32>=<NULL>
    "svce"<blob>="Signal Safe Storage"
    "type"<uint32>=<NULL>



    mqqCXCU/iaszlgACa4aWZg==


{
  "encryptedKey": "763130345418473a12fbfd3a7fa4aa6baa7c4fe9069b2b09f28a4caf1dfcd280643333064839e596d18167225eba0ecd0c6245050a5e2ff73fd7319087443faca8e72debf94b70dd6732b5e979dc7cfd8f5318"
}

*/

char *CFStringCopyUTF8String(CFStringRef cfsr)
{
  if (!cfsr)
    return nullptr;

  CFIndex length = CFStringGetLength(cfsr);
  CFIndex max_length = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;
  char *buffer = new char[max_length];
  if (CFStringGetCString(cfsr, buffer, max_length, kCFStringEncodingUTF8))
    return buffer;
  delete[] buffer;
  return nullptr;
}

void getSecret_Mac(std::set<std::string> *secrets)
{
  int const dict_size = 4;
  void const *keys[dict_size] = {kSecClass,
				 kSecAttrAccount,
				 kSecAttrService,
				 kSecReturnData};
  CFStringRef account = CFStringCreateWithCString(nullptr, "Signal Key", kCFStringEncodingUTF8);
  CFStringRef service = CFStringCreateWithCString(nullptr, "Signal Safe Storage", kCFStringEncodingUTF8);
  void const *values[dict_size] = {kSecClassGenericPassword,
				   account,
				   service,
				   kCFBooleanTrue};
  CFDictionaryRef query = CFDictionaryCreate(nullptr, keys, values, dict_size, nullptr, nullptr);

  CFDataRef item_data = nullptr;
  OSStatus ret = SecItemCopyMatching(query, reinterpret_cast<CFTypeRef *>(&item_data));

  // release some refs...
  CFRelease(query);
  CFRelease(account);
  CFRelease(service);

  if (ret != 0)
  {
    std::cout << ret << std::endl;
    char *str = CFStringCopyUTF8String(SecCopyErrorMessageString(ret, nullptr));
    if (str)
    {
      std::cout << str << std::endl;
      delete[] str;
    }
    return;
  }

  // parse returned items...
  int password_length = CFDataGetLength(item_data);
  if (password_length != 24)
  {
    std::cout << "Unexpected password length" << std::endl;
    return;
  }
  std::string password(reinterpret_cast<char const *>(CFDataGetBytePtr(item_data)), password_length);

  if (g_verbose) [[unlikely]] std::cout << "Got password: " << password << std::endl;

  secrets->emplace(password);

  return;
}
