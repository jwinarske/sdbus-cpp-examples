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

#ifndef SRC_UTILS_LOGGING_H
#define SRC_UTILS_LOGGING_H

/**
 * @brief Centralized logging header - SINGLE PLACE spdlog.h is included
 *
 * This header consolidates all logging infrastructure:
 * - spdlog.h (the only place it's included from)
 * - Logging configuration (environment-based setup)
 * - Error reporting macros (standardized logging levels)
 *
 * All other files should include this header instead of including spdlog
 * directly.
 *
 * Usage:
 *   #include "src/utils/logging.h"
 *
 *   LOG_INFO("Message");
 *   LOG_DEBUG("Debug info");
 *   LOG_ERROR("Error occurred");
 */

// ============================================================================
// SINGLE INCLUDE OF SPDLOG - ONLY LOCATION IN CODEBASE
// ============================================================================
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

// ============================================================================
// LOGGING CONFIGURATION - Environment-based setup
// ============================================================================

namespace logging_config {

// ============================================================================
// INPUT VALIDATION
// ============================================================================

/**
 * @brief Validate a log file path supplied via the LOG_FILE environment
 *        variable.
 *
 * Rejects values that could be used for path-traversal or other attacks:
 *   - Paths containing ".." components
 *   - Paths containing null bytes
 *   - Paths containing newline / carriage-return characters
 *   - Paths that start with '-' (would be misinterpreted as flags)
 *
 * @param path  Candidate path string.
 * @return true  if the path is considered safe to use.
 * @return false if the path is rejected; a warning is emitted.
 */
inline bool isLogFilePathSafe(const std::string& path) {
  if (path.empty()) {
    return true;  // empty == "no file logging", always safe
  }

  // Null bytes anywhere in the string
  if (path.find('\0') != std::string::npos) {
    spdlog::warn("LOG_FILE rejected: contains null byte");
    return false;
  }

  // Newline / carriage-return (log-injection vector)
  if (path.find('\n') != std::string::npos ||
      path.find('\r') != std::string::npos) {
    spdlog::warn("LOG_FILE rejected: contains newline character");
    return false;
  }

  // Leading dash (could be misinterpreted as a flag by some open() wrappers)
  if (path.front() == '-') {
    spdlog::warn("LOG_FILE rejected: starts with '-'");
    return false;
  }

  // Path-traversal: reject any ".." component
  // Walk the string looking for the sequence  (/ | start) ".." (/ | end)
  const std::string_view sv(path);
  std::string_view::size_type pos = 0;
  while (pos < sv.size()) {
    // find the next slash or end
    const auto slash = sv.find('/', pos);
    const std::string_view component =
        sv.substr(pos, slash == std::string_view::npos ? std::string_view::npos
                                                       : slash - pos);
    if (component == "..") {
      spdlog::warn("LOG_FILE rejected: path traversal component '..' detected");
      return false;
    }
    if (slash == std::string_view::npos) {
      break;
    }
    pos = slash + 1;
  }

  return true;
}

/**
 * @brief Validate a log pattern supplied via the LOG_PATTERN environment
 *        variable.
 *
 * spdlog patterns use %-prefixed tokens (e.g. %Y, %H, %l, %v).  An attacker
 * who controls the pattern string cannot trigger printf-style vulnerabilities
 * (spdlog does not use printf internally), but an unexpected pattern can:
 *   - Cause spdlog to crash / behave unexpectedly with unknown tokens
 *   - Inject ANSI escape sequences or other control characters
 *
 * We allow only the printable ASCII characters that appear in typical spdlog
 * patterns and reject any non-printable or non-ASCII byte.
 *
 * @param pattern  Candidate pattern string.
 * @return true  if the pattern is considered safe.
 * @return false if the pattern is rejected; a warning is emitted.
 */
inline bool isLogPatternSafe(const std::string& pattern) {
  if (pattern.empty()) {
    return true;  // empty == use default, always safe
  }

  for (const unsigned char ch : pattern) {
    // Allow printable ASCII only (0x20–0x7E) plus tab (0x09)
    if (ch != 0x09 && (ch < 0x20 || ch > 0x7E)) {
      spdlog::warn(
          "LOG_PATTERN rejected: contains non-printable or non-ASCII byte "
          "(0x{:02X})",
          static_cast<unsigned>(ch));
      return false;
    }
  }

  return true;
}

/**
 * @brief Get log level from the environment variable or default to info
 * @return spdlog::level::level_enum
 */
inline spdlog::level::level_enum getLogLevelFromEnv() {
  const char* level_str = std::getenv("LOG_LEVEL");
  if (!level_str) {
    return spdlog::level::info;
  }

  const std::string level{level_str};
  if (level == "trace")
    return spdlog::level::trace;
  if (level == "debug")
    return spdlog::level::debug;
  if (level == "info")
    return spdlog::level::info;
  if (level == "warn")
    return spdlog::level::warn;
  if (level == "err")
    return spdlog::level::err;
  if (level == "critical")
    return spdlog::level::critical;
  if (level == "off")
    return spdlog::level::off;

  // Unknown value — fall back to info and warn the operator
  spdlog::warn("LOG_LEVEL '{}' is not recognized; defaulting to 'info'", level);
  return spdlog::level::info;
}

/**
 * @brief Check if file logging is enabled via an environment variable.
 *
 * The path is validated by isLogFilePathSafe() before being returned.
 * An unsafe path is treated the same as an absent variable (no file logging).
 *
 * @return std::string validated path to a log file, or empty string.
 */
inline std::string getLogFilePathFromEnv() {
  const char* log_file = std::getenv("LOG_FILE");
  if (!log_file) {
    return {};
  }
  std::string path(log_file);
  if (!isLogFilePathSafe(path)) {
    // Warning already emitted by isLogFilePathSafe(); fall back to no file.
    return {};
  }
  return path;
}

/**
 * @brief Initialize logging with console and optional file output
 *
 * Finding 8 — SPDLOG_NO_EXCEPTIONS ON: with exceptions disabled spdlog routes
 * internal errors (bad sink path, disk full, etc.) through its error handler
 * instead of throwing.  We install a custom handler that writes to stderr so
 * that failures are never silently dropped.  The handler is set *before* any
 * sink is constructed so that even sink-construction failures are visible.
 *
 * @param logger_name Name of the logger instance
 */
inline void initializeLogging(const std::string& logger_name = "default") {
  // Install the error handler first so sink-construction failures are visible.
  // This is the *only* reliable notification path when SPDLOG_NO_EXCEPTIONS ON.
  spdlog::set_error_handler([](const std::string& msg) {
    // Cannot use spdlog itself here (we may be inside spdlog).
    // Write directly to stderr so the failure is always visible.
    std::fprintf(stderr, "[spdlog error] %s\n", msg.c_str());
  });

  try {
    const auto level = getLogLevelFromEnv();
    const auto log_file = getLogFilePathFromEnv();

    // Validate LOG_PATTERN; fall back to the safe built-in default if unsafe.
    static constexpr std::string_view kDefaultPattern =
        "[%Y-%m-%d %H:%M:%S.%e] [%l] %v";
    const char* pattern_env = std::getenv("LOG_PATTERN");
    std::string pattern;
    if (pattern_env) {
      const std::string candidate(pattern_env);
      pattern = isLogPatternSafe(candidate) ? candidate
                                            : std::string(kDefaultPattern);
    } else {
      pattern = std::string(kDefaultPattern);
    }

    const auto console_sink =
        std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(level);

    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(console_sink);

    if (!log_file.empty()) {
      const auto file_sink =
          std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
              log_file, 10 * 1024 * 1024, 3);
      file_sink->set_level(level);
      sinks.push_back(file_sink);
    }

    const auto logger = std::make_shared<spdlog::logger>(
        logger_name, sinks.begin(), sinks.end());
    logger->set_level(level);
    logger->set_pattern(pattern);
    logger->flush_on(spdlog::level::err);

    spdlog::register_logger(logger);
    spdlog::set_default_logger(logger);

    spdlog::info("Logging initialized - Level: {}, File: {}",
                 spdlog::level::to_string_view(level),
                 log_file.empty() ? "console only" : log_file);
  } catch (const spdlog::spdlog_ex& ex) {
    // This branch is reached when SPDLOG_NO_EXCEPTIONS is OFF.
    // Mirror the error-handler path so the failure is always on stderr.
    std::fprintf(stderr, "[spdlog error] Log initialization failed: %s\n",
                 ex.what());
    spdlog::error("Log initialization failed: {}", ex.what());
  }
}

// Helpers for callers that need runtime level/flush configuration without
// referencing spdlog symbols directly.
inline void setLevelDebug() {
  spdlog::set_level(spdlog::level::debug);
}
inline void setLevelInfo() {
  spdlog::set_level(spdlog::level::info);
}
inline void setFlushInterval(const std::chrono::seconds interval) {
  spdlog::flush_every(interval);
}

}  // namespace logging_config

// ============================================================================
// ERROR REPORTING MACROS - Standardized logging levels
// ============================================================================

/**
 * @brief TRACE - Development only: detailed traces, function entry/exit
 */
#define LOG_TRACE(...) spdlog::trace(__VA_ARGS__)

/**
 * @brief DEBUG - Development/troubleshooting: state changes, discoveries
 */
#define LOG_DEBUG(...) spdlog::debug(__VA_ARGS__)

/**
 * @brief INFO - Normal operation: startup/shutdown, significant events
 * (DEFAULT)
 */
#define LOG_INFO(...) spdlog::info(__VA_ARGS__)

/**
 * @brief WARN - Recoverable errors: retries, degradation, resource warnings
 */
#define LOG_WARN(...) spdlog::warn(__VA_ARGS__)

/**
 * @brief ERROR - Critical failures: hard limits, invariant violations
 */
#define LOG_ERROR(...) spdlog::error(__VA_ARGS__)

/**
 * @brief CRITICAL - Fatal conditions: data corruption, immediate shutdown
 */
#define LOG_CRITICAL(...) spdlog::critical(__VA_ARGS__)

// ============================================================================
// ENVIRONMENT VARIABLES FOR LOGGING CONFIGURATION
// ============================================================================
//
// LOG_LEVEL={trace,debug,info,warn,err,critical,off} - Set log level (default:
// info) LOG_FILE=/path/to/file - Enable file logging with automatic rotation
// LOG_PATTERN="[%t] [%l] %v" - Custom spdlog pattern string
//
// Examples:
//   LOG_LEVEL=debug ./app
//   LOG_FILE=/var/log/app.log LOG_LEVEL=info ./app
//   LOG_PATTERN="[%H:%M:%S] [%l] %v" LOG_LEVEL=debug ./app

// ============================================================================
// ERROR REPORTING GUIDELINES - Standardized logging severity reference
// ============================================================================
//
// LOG_TRACE  - Development only: detailed traces, function entry/exit
// LOG_DEBUG  - Development/troubleshooting: state changes, discoveries
// LOG_INFO   - Normal operation: startup/shutdown, significant events (DEFAULT)
// LOG_WARN   - Recoverable errors: retries, degradation, resource warnings
// LOG_ERROR  - Fatal errors: critical failures, hard limits, invariants broken
// LOG_CRITICAL - Application fatal: data corruption, immediate shutdown
//
// Decision tree for choosing level:
//   Is this a debug/internal detail? -> LOG_DEBUG
//   Is this a normal operation milestone? -> LOG_INFO
//   Is the error recoverable/expected to retry? -> LOG_WARN
//   Can the operation continue? NO -> LOG_ERROR, YES -> LOG_WARN
//   Is this application-fatal? -> LOG_CRITICAL before exit

#endif  // SRC_UTILS_LOGGING_H
