#ifndef SUBGHZDECODER_LOGGER_LOGGER_H_
#define SUBGHZDECODER_LOGGER_LOGGER_H_

enum class LogSeverity {
  DEBUG,
  INFO,
  WARNING,
  ERROR
};

class Logger {
  public:
    static void log(LogSeverity severity, const char *message, bool isFirst = false);
};

#endif /* SUBGHZDECODER_LOGGER_LOGGER_H_ */