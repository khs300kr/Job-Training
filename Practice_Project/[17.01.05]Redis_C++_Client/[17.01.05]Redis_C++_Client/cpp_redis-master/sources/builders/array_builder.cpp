#include <cpp_redis/builders/array_builder.hpp>
#include <cpp_redis/builders/builders_factory.hpp>
#include <cpp_redis/logger.hpp>
#include <cpp_redis/redis_error.hpp>

namespace cpp_redis {

namespace builders {

array_builder::array_builder(void)
: m_current_builder(nullptr)
, m_reply_ready(false)
, m_reply(std::vector<reply>{}) {}

bool
array_builder::fetch_array_size(std::string& buffer) {
  if (m_int_builder.reply_ready())
    return true;

  m_int_builder << buffer;
  if (!m_int_builder.reply_ready())
    return false;

  int64_t size = m_int_builder.get_integer();
  if (size < 0) {
    __CPP_REDIS_LOG(error, "cpp_redis::builders::array_builder receives invalid array size");
    throw redis_error("Invalid array size");
  }
  else if (size == 0)
    m_reply_ready = true;

  m_array_size = size;

  return true;
}

bool
array_builder::build_row(std::string& buffer) {
  if (!m_current_builder) {
    m_current_builder = create_builder(buffer.front());
    buffer.erase(0, 1);
  }

  *m_current_builder << buffer;
  if (!m_current_builder->reply_ready())
    return false;

  m_reply << m_current_builder->get_reply();
  m_current_builder = nullptr;

  if (m_reply.as_array().size() == m_array_size)
    m_reply_ready = true;

  return true;
}

builder_iface&
array_builder::operator<<(std::string& buffer) {
  if (m_reply_ready)
    return *this;

  if (!fetch_array_size(buffer))
    return *this;

  while (buffer.size() && !m_reply_ready)
    if (!build_row(buffer))
      return *this;

  return *this;
}

bool
array_builder::reply_ready(void) const {
  return m_reply_ready;
}

reply
array_builder::get_reply(void) const {
  return reply{m_reply};
}

} //! builders

} //! cpp_redis
