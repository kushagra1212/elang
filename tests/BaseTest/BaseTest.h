#ifndef BASE_TEST_H
#define BASE_TEST_H

#include <sstream>
#include <string>

class BaseTest {
public:
  virtual void SetUp() = 0;
  virtual void TearDown() = 0;
  virtual void setInput(const std::string &input) = 0;
  virtual void runEvaluator() = 0;
  virtual std::string getOutput() const = 0;

  std::stringstream input_stream;
  std::stringstream output_stream;
  std::streambuf *saved_cout_buf;
};

#endif // BASE_TEST_H