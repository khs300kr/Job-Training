#include <cpp_redis/builders/error_builder.hpp>

namespace cpp_redis {

namespace builders {

builder_iface&
error_builder::operator<<(std::string& buffer) {
  m_string_builder << buffer;

  if (m_string_builder.reply_ready())
    m_reply.set(m_string_builder.get_simple_string(), reply::string_type::error);

  return *this;
}

bool
error_builder::reply_ready(void) const {
  return m_string_builder.reply_ready();
}

reply
error_builder::get_reply(void) const {
  return reply{m_reply};
}

const std::string&
error_builder::get_error(void) const {
  return m_string_builder.get_simple_string();
}

} //! builders

} //! cpp_redis
