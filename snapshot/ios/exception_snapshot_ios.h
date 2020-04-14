// Copyright 2020 The Crashpad Authors. All rights reserved.
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

#ifndef CRASHPAD_SNAPSHOT_IOS_EXCEPTION_SNAPSHOT_IOS_H_
#define CRASHPAD_SNAPSHOT_IOS_EXCEPTION_SNAPSHOT_IOS_H_

#include <mach/mach.h>
#include <stdint.h>

#include <vector>

#include "base/macros.h"
#include "build/build_config.h"
#include "snapshot/cpu_context.h"
#include "snapshot/exception_snapshot.h"
#include "util/mach/mach_extensions.h"
#include "util/misc/initialization_state_dcheck.h"

namespace crashpad {

namespace internal {

//! \brief An ExceptionSnapshot of an exception sustained by a running (or
//!     crashed) process on an iOS system.
class ExceptionSnapshotIOS final : public ExceptionSnapshot {
 public:
  ExceptionSnapshotIOS();
  ~ExceptionSnapshotIOS() override;

  //! \brief Initializes the object.
  //!
  //! \return `true` if the snapshot could be created, `false` otherwise with
  //!     an appropriate message logged.
  bool Initialize(const siginfo_t* siginfo, const ucontext_t* context);

  // ExceptionSnapshot:

  const CPUContext* Context() const override;
  uint64_t ThreadID() const override;
  uint32_t Exception() const override;
  uint32_t ExceptionInfo() const override;
  uint64_t ExceptionAddress() const override;
  const std::vector<uint64_t>& Codes() const override;
  virtual std::vector<const MemorySnapshot*> ExtraMemory() const override;

 private:
#if defined(ARCH_CPU_X86_64)
  CPUContextX86_64 context_x86_64_;
#elif defined(ARCH_CPU_ARM64)
  CPUContextARM64 context_arm64_;
#else
#error Port.
#endif  // ARCH_CPU_X86_64
  CPUContext context_;
  std::vector<uint64_t> codes_;
  uint64_t thread_id_;
  uintptr_t exception_address_;
  int signal_number_;
  int signal_code_;
  InitializationStateDcheck initialized_;

  DISALLOW_COPY_AND_ASSIGN(ExceptionSnapshotIOS);
};

}  // namespace internal
}  // namespace crashpad

#endif  // CRASHPAD_SNAPSHOT_IOS_EXCEPTION_SNAPSHOT_IOS_H_
