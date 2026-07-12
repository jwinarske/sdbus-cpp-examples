// Copyright (c) 2026 Joel Winarske
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef SRC_UTILS_EVENT_LOOP_H
#define SRC_UTILS_EVENT_LOOP_H

#include <atomic>
#include <memory>
#include <vector>

#include <poll.h>

#include "unique_fd.h"

namespace sdbus {
class IConnection;
}

/// A file descriptor that can be multiplexed by EventLoop.
///
/// fd() is polled for events() (POLLIN by default); when ready, dispatch() is
/// invoked on the loop thread with the reported revents. Because every source
/// is dispatched from the single loop thread, state touched only from
/// dispatch() needs no locking.
class EventSource {
 public:
  virtual ~EventSource() = default;

  [[nodiscard]] virtual int fd() const = 0;
  [[nodiscard]] virtual short events() const { return POLLIN; }

  /// Called on the loop thread when fd() is ready. Must not block.
  virtual void dispatch(short revents) = 0;
};

/// Single-threaded event loop that multiplexes an sdbus::IConnection together
/// with a set of EventSource objects, so D-Bus callbacks and device/fd I/O all
/// run on one thread. This is the alternative to enterEventLoopAsync(): drive
/// the bus from our own poll() instead of a bus-owned thread, which removes the
/// need to synchronise state shared between the D-Bus and device callbacks.
///
/// Threading: run() and all EventSource::dispatch() callbacks execute on the
/// thread that calls run(). add()/remove() are intended to be called from that
/// same thread (typically from within a dispatch()); they take effect on the
/// next loop iteration. stop() is the sole exception — it is safe to call from
/// a signal handler or another thread.
class EventLoop {
 public:
  EventLoop();

  EventLoop(const EventLoop&) = delete;
  EventLoop& operator=(const EventLoop&) = delete;
  EventLoop(EventLoop&&) = delete;
  EventLoop& operator=(EventLoop&&) = delete;
  ~EventLoop() = default;

  /// Register a source. Takes effect on the next loop iteration, so it is safe
  /// to call from within a dispatch() callback.
  void add(EventSource* source);

  /// Unregister a source. Takes effect on the next loop iteration. The source
  /// object must remain alive until then, so do not destroy it inside its own
  /// dispatch() — reset it after the next iteration (or after run() returns).
  void remove(EventSource* source);

  /// Unregister a source and take ownership of destroying it at a safe point
  /// (after the current dispatch pass completes). Use this to drop a source
  /// from within a callback — e.g. a udev "remove" handler retiring the reader
  /// for the device that just went away — without risking a use-after-free on
  /// a source whose fd is still in the poll set for the current iteration.
  void retire(std::unique_ptr<EventSource> source);

  /// Drive `bus` and every registered source until stop() is called or an
  /// unrecoverable error occurs. Returns the code passed to stop() (0 on a
  /// clean external stop, non-zero on internal error).
  int run(sdbus::IConnection& bus);

  /// Request the loop to exit with `exit_code`. Async-signal-safe and
  /// thread-safe: writes an eventfd to break out of poll().
  void stop(int exit_code = 0) noexcept;

 private:
  void wake() noexcept;
  void apply_pending();

  UniqueFd
      wake_fd_;  // eventfd used to break out of poll() for stop()/add/remove
  std::atomic<bool> running_{false};
  std::atomic<int> exit_code_{0};

  std::vector<EventSource*> sources_;
  std::vector<EventSource*> to_add_;
  std::vector<EventSource*> to_remove_;
  // Sources awaiting destruction; kept alive until the next apply_pending() so
  // a source retired mid-dispatch outlives the current poll iteration.
  std::vector<std::unique_ptr<EventSource>> to_retire_;
};

#endif  // SRC_UTILS_EVENT_LOOP_H
