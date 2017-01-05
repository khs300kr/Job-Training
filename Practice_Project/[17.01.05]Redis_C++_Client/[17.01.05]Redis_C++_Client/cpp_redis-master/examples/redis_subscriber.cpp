#include <cpp_redis/cpp_redis>

#include <iostream>
#include <signal.h>

volatile std::atomic_bool should_exit(false);

void
sigint_handler(int) {
  should_exit = true;
}

int
main(void) {
  //! Enable logging
  cpp_redis::active_logger = std::unique_ptr<cpp_redis::logger>(new cpp_redis::logger);

  cpp_redis::redis_subscriber sub;

  sub.connect("127.0.0.1", 6379, [](cpp_redis::redis_subscriber&) {
    std::cout << "sub disconnected (disconnection handler)" << std::endl;
    should_exit = true;
  });

  sub.subscribe("some_chan", [](const std::string& chan, const std::string& msg) {
    std::cout << "MESSAGE " << chan << ": " << msg << std::endl;
  });
  sub.psubscribe("*", [](const std::string& chan, const std::string& msg) {
    std::cout << "PMESSAGE " << chan << ": " << msg << std::endl;
  });
  sub.commit();

  signal(SIGINT, &sigint_handler);
  while (!should_exit) {}

  return 0;
}
