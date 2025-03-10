
/*
 * This file was automatically generated by sdbus-c++-xml2cpp; DO NOT EDIT!
 */

#ifndef __sdbuscpp__src_proxy_org_freedesktop_locale1_locale1_proxy_h__proxy__H__
#define __sdbuscpp__src_proxy_org_freedesktop_locale1_locale1_proxy_h__proxy__H__

#include <sdbus-c++/sdbus-c++.h>
#include <string>
#include <tuple>

namespace org {
namespace freedesktop {

class locale1_proxy {
 public:
  static constexpr const char* INTERFACE_NAME = "org.freedesktop.locale1";

 protected:
  locale1_proxy(sdbus::IProxy& proxy) : m_proxy(proxy) {}

  locale1_proxy(const locale1_proxy&) = delete;
  locale1_proxy& operator=(const locale1_proxy&) = delete;
  locale1_proxy(locale1_proxy&&) = delete;
  locale1_proxy& operator=(locale1_proxy&&) = delete;

  ~locale1_proxy() = default;

  void registerProxy() {}

 public:
  void SetLocale(const std::vector<std::string>& locale,
                 const bool& interactive) {
    m_proxy.callMethod("SetLocale")
        .onInterface(INTERFACE_NAME)
        .withArguments(locale, interactive);
  }

  void SetVConsoleKeyboard(const std::string& keymap,
                           const std::string& keymap_toggle,
                           const bool& convert,
                           const bool& interactive) {
    m_proxy.callMethod("SetVConsoleKeyboard")
        .onInterface(INTERFACE_NAME)
        .withArguments(keymap, keymap_toggle, convert, interactive);
  }

  void SetX11Keyboard(const std::string& layout,
                      const std::string& model,
                      const std::string& variant,
                      const std::string& options,
                      const bool& convert,
                      const bool& interactive) {
    m_proxy.callMethod("SetX11Keyboard")
        .onInterface(INTERFACE_NAME)
        .withArguments(layout, model, variant, options, convert, interactive);
  }

 public:
  std::vector<std::string> Locale() {
    return m_proxy.getProperty("Locale")
        .onInterface(INTERFACE_NAME)
        .get<std::vector<std::string>>();
  }

  std::string X11Layout() {
    return m_proxy.getProperty("X11Layout")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string X11Model() {
    return m_proxy.getProperty("X11Model")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string X11Variant() {
    return m_proxy.getProperty("X11Variant")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string X11Options() {
    return m_proxy.getProperty("X11Options")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string VConsoleKeymap() {
    return m_proxy.getProperty("VConsoleKeymap")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

  std::string VConsoleKeymapToggle() {
    return m_proxy.getProperty("VConsoleKeymapToggle")
        .onInterface(INTERFACE_NAME)
        .get<std::string>();
  }

 private:
  sdbus::IProxy& m_proxy;
};

}  // namespace freedesktop
}  // namespace org

#endif
