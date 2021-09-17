// Copyright (c) 2010 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Modified by Adobe Systems Incorporated [2012]

#ifndef SIMPLE_IPC_OS_INCLUDES_H_
#define SIMPLE_IPC_OS_INCLUDES_H_

#if defined(WIN32)
//////////////////////////////// Windows //////////////////////////////////////////////////////////
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0603     
#endif

#include <windows.h>

#else
//////////////////////////////// Other OS /////////////////////////////////////////////////////////
#include <stdlib.h>

#endif  // defined(WIN32)

//////////////////////////////// Every OS /////////////////////////////////////////////////////////

#ifndef IPC_USE_STL
#	define IPC_USE_STL
#endif

// changes made by adobe are guarded against ACROBAT_SANDBOX macro
#ifndef ACROBAT_SANDBOX
#define ACROBAT_SANDBOX 1
#endif

#if defined(IPC_USE_STL)
  #include <string>
  #include <vector>
  typedef std::string IPCString;
  typedef std::wstring IPCWString;
  typedef std::vector<char> IPCCharVector;
  typedef std::vector<void*> IPCVoidPtrVector;
  typedef std::vector<int> IPCIntVector;
#if  ACROBAT_SANDBOX 
  typedef std::vector<int64_t> IPCInt64Vector;
#endif
#else
  #include "ipc_utils.h"
  typedef ipc::HolderString<char> IPCString;
  typedef ipc::HolderString<wchar_t> IPCWString;
  typedef ipc::PodVector<char> IPCCharVector;
  typedef ipc::PodVector<void*> IPCVoidPtrVector;
  typedef ipc::PodVector<int> IPCIntVector;
#endif


#endif  // SIMPLE_IPC_OS_INCLUDES_H_
