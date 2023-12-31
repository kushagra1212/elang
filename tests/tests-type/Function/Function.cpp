
#include "Function.h"

Function::Function() {
#ifdef JIT_TEST_MODE
  _test = std::make_unique<JITCompilerTest>();
#endif

#ifdef REPL_TEST_MODE
  _test = std::make_unique<ReplTest>();
#endif
}

void Function::SetUp() { _test->SetUp(); }

void Function::TearDown() { _test->TearDown(); }

void Function::setInput(const std::string &input) { _test->setInput(input); }

std::string Function::getOutput() const { return _test->getOutput(); }

void Function::runEvaluator() { _test->runEvaluator(); }

// Function with no parameters

// Function with only return statement

// nthg is the return type and int str bool deci nthg are the return value

#ifndef JIT_TEST_MODE

TEST_F(Function, NthgReturnTypeWithIntReturnValue) {
  std::string input = R"(
fun main()-> nthg {
    return 2
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      "Function return type is Nothing, return expression is found";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);

// check expected output is substring of the output or not
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}
TEST_F(Function, NthgReturnTypeWithStringReturnValue) {
  std::string input = R"(
    fun main()-> nthg {
      return "Hello"
    }
    print(main())
  )";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Function return type is Nothing, return expression is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, NthgReturnTypeWithBoolReturnValue) {
  std::string input = R"(fun main()-> nthg {
    return (true)
}
print(main()))";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Function return type is Nothing, return expression is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}
TEST_F(Function, NthgReturnTypeWithDeciReturnValue) {
  std::string input = R"(fun main()-> nthg {
    return 2.2
}
print(main()))";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Function return type is Nothing, return expression is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

// Int return type with int str bool deci nthg as return value

TEST_F(Function, IntReturnTypeWithNoReturnValue) {
  std::string input = R"(fun main()-> int {
    return 2+2
}
print(main()))";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::string expected_output = "4";

  EXPECT_EQ(lowerCaseOutput, expected_output);
}

TEST_F(Function, IntReturnTypeWithNoReturnValueWithBrackets) {
  std::string input = R"(fun main()-> int {
    return (2+2)
}
print(main()))";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::string expected_output = "4";

  EXPECT_EQ(lowerCaseOutput, expected_output);
}

TEST_F(Function, IntReturnTypeWithNthg) {
  std::string input = R"(
fun main()-> int {
    return:
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Function return type is not Nothing, return expression is not found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}
TEST_F(Function, IntReturnTypeWithNoReturn) {
  std::string input = R"(fun main2()-> int {
}print(main2()))";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Function return type is not Nothing, return expression is not found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, IntReturnTypeWithStrReturnValue) {
  std::string input = R"(
fun main()-> int {
return "str"
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Return Type Mismatch Int32 is expected but String is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, IntReturnTypeWithBoolReturnValue) {
  std::string input = R"(
fun main()-> int {
return true
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Return Type Mismatch Int32 is expected but Bool is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);

#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, IntReturnTypeWithBoolReturnValueFalse) {
  std::string input = R"(
fun main()-> int {
return false
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Return Type Mismatch Int32 is expected but Bool is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}
TEST_F(Function, IntReturnTypeWithDeciReturnValue) {
  std::string input = R"(
fun main()-> int {
return 1.2
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Return Type Mismatch Int32 is expected but Decimal is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

// Bool Return Type

TEST_F(Function, BoolReturnTypeWithIntReturnValue) {
  std::string input = R"(
fun main()-> bool {
return 1
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Return Type Mismatch Bool is expected but Int32 is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, BoolReturnTypeWithNthgReturnValue) {
  std::string input = R"(
fun main()-> bool {
return:
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Function return type is not Nothing, return expression is not found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, BoolReturnTypeWithNoReturn) {
  std::string input = R"(
fun main()-> bool {

}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Function return type is not Nothing, return expression is not found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, BoolReturnTypeWithStringReturnValue) {
  std::string input = R"(
fun main()-> bool {
return "str"
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Return Type Mismatch Bool is expected but String is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, BoolReturnTypeWithBoolReturnValueTrue) {
  std::string input = R"(
fun main()-> bool {
return true
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::string expected_output = R"(true)";

  EXPECT_EQ(lowerCaseOutput, expected_output);
}

TEST_F(Function, BoolReturnTypeWithBoolReturnValueFalse) {
  std::string input = R"(
fun main()-> bool {
return false
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::string expected_output = R"(false)";

  EXPECT_EQ(lowerCaseOutput, expected_output);
}

TEST_F(Function, BoolReturnTypeWithDeciReturnValue) {
  std::string input = R"(
fun main()-> bool {
return 1.2
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Return Type Mismatch Bool is expected but Decimal is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

// String Return Type

TEST_F(Function, StringReturnTypeWithIntReturnValue) {
  std::string input = R"(
fun main()-> str {
return 1
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Return Type Mismatch String is expected but Int32 is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, StringReturnTypeWithNthgReturnValue) {
  std::string input = R"(
fun main()-> str {
return:
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Function return type is not Nothing, return expression is not found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, StringReturnTypeWithNoReturnValue) {
  std::string input = R"(
fun main()-> str {

}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Function return type is not Nothing, return expression is not found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, StringReturnTypeWithBoolReturnValueTrue) {
  std::string input = R"(
fun main()-> str {
return true
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Return Type Mismatch String is expected but Bool is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, StringReturnTypeWithBoolReturnValueFalse) {
  std::string input = R"(
fun main()-> str {
return (false)
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Return Type Mismatch String is expected but Bool is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, StringReturnTypeWithDeciReturnValue) {
  std::string input = R"(
fun main()-> str {
return 1.2
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Return Type Mismatch String is expected but Decimal is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, NthgReturnTypeWithNthgReturnValue) {
  std::string input = R"(
fun main()-> nthg {
    return:
}
main()
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();
  std::string expected_output;
#ifdef JIT_TEST_MODE
  expected_output = "";
#elif REPL_TEST_MODE
  expected_output = "";
#endif

  EXPECT_EQ(lowerCaseOutput, expected_output);
}
TEST_F(Function, NthgReturnTypeWithNoReturnValue) {
  std::string input = R"(fun main()-> nthg {
  
}
main())";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::string expected_output;
#ifdef JIT_TEST_MODE
  expected_output = "";
#elif REPL_TEST_MODE
  expected_output = "";
#endif

  EXPECT_EQ(lowerCaseOutput, expected_output);
}

TEST_F(Function, StringReturnTypeWithStringReturnValue) {
  std::string input = R"(
fun main()-> str {
return "Hello" + " World"
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::string expected_output = R"(Hello World)";

  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// sum of n numbers using recursion

TEST_F(Function, SumOfNNumbersUsingRecursion) {
  std::string input = R"(
fun sum(n) -> int {
  if (n == 0) {
    return 0
  }
  return n + sum(n - 1)
}
print(sum(10))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "55";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// sum of n numbers using loop

TEST_F(Function, SumOfNNumbersUsingLoop) {
  std::string input = R"(
fun sum(n) -> int {
  var sum = 0
  for (var i = 0 to n : 1) {
    sum = sum + i
  }
  return sum
}
print(sum(10))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "55";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// write a function to find the factorial of a number

TEST_F(Function, FactorialOfANumberUsingRecursion) {
  std::string input = R"(
fun factorial(n) -> int {
  if (n == 0) {
    return 1
  }
  return n * factorial(n - 1)
}
print(factorial(5))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "120";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// write a function to find the factorial of a number using loop

TEST_F(Function, FactorialOfANumberUsingLoop) {
  std::string input = R"(
fun factorial(n) -> int {
  var fact = 1
  for (var i = 1 to n : 1) {
    fact = fact * i
  }
  return fact
}
print(factorial(5))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "120";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// write a function to find the fibonacci of a number

TEST_F(Function, FibonacciOfANumberUsingRecursion) {
  std::string input = R"(
fun fibonacci(n) -> int {
  if (n == 0) {
    return 0
  }
  if (n == 1) {
    return 1
  }
  return fibonacci(n - 1) + fibonacci(n - 2)
}
print(fibonacci(10))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "55";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// write a function to find the fibonacci of a number using loop

TEST_F(Function, FibonacciOfANumberUsingLoop) {
  std::string input = R"(
fun fibonacci(n) -> int {
  var a = 0
  var b = 1
  var c = 0
  for (var i = 2 to n : 1) {
    c = a + b
    a = b
    b = c
  }
  return c
}
print(fibonacci(10))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "55";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// write a function to find the gcd of two numbers

TEST_F(Function, GCDOfTwoNumbersUsingRecuhrsion) {
  std::string input = R"(
fun gcd(a, b) -> int {
  if (b == 0) {
    return a
  }
  return gcd(b, a % b)
}
print(gcd(10, 15))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "5";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// write a function to find the gcd of two numbers using loop

TEST_F(Function, GCDOfTwoNumbersUsingLoop) {
  std::string input = R"(
fun gcd(a, b) -> int {
  while (b != 0) {
    var temp = b
    b = a % b
    a = temp
  }
  return a
}
print(gcd(10, 15))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "5";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// write a function to find the lcm of two numbers

TEST_F(Function, LCMOfTwoNumbersUsingRecursion) {
  std::string input = R"(
fun gcd(a, b) -> int {
  if (b == 0) {
    return a
  }
  return gcd(b, a % b)
}
fun lcm(a, b) -> int {
  return ((a * b) // gcd(a, b))
}
print(lcm(10, 15))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "30";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// write a function to find the lcm of two numbers using loop

TEST_F(Function, LCMOfTwoNumbersUsingLoop) {
  std::string input = R"(
fun gcd(a, b) -> int {
  while (b != 0) {
    var temp = b
    b = a % b
    a = temp
  }
  return a
}

fun lcm(a, b) -> int {
  return ((a * b) // gcd(a, b))
}

print(lcm(10, 15))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "30";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// write a function to find the power of a number

TEST_F(Function, PowerOfANumberUsingRecursion) {
  std::string input = R"(
fun power(base, exp) -> int {
  if (exp == 0) {
    return 1
  }
  return base * power(base, exp - 1)
}

print(power(2, 3))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "8";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// write a function to find the power of a number using loop

TEST_F(Function, PowerOfANumberUsingLoop) {
  std::string input = R"(
fun power(base, exp) -> int {
  var result = 1
  for (var i = 0 to exp - 1: 1) {
    result = result * base
  }
  return result
}

print(power(2, 3))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "8";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// write a function to find the sum of digits of a number

TEST_F(Function, SumOfDigitsOfANumberUsingRecursion) {
  std::string input = R"(

fun sumOfDigits(n) -> int {
  if (n == 0) {
    return 0
  }
  return ((n % 10) + sumOfDigits(n // 10))
}

print(sumOfDigits(123))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "6";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// write a function to find the sum of digits of a number using loop

TEST_F(Function, SumOfDigitsOfANumberUsingLoop) {
  std::string input = R"(
fun sumOfDigits(n) -> int {
  var sum = 0
  while (n != 0) {
    sum = sum + (n % 10)
    n = n // 10
  }
  return sum
}

print(sumOfDigits(123))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "6";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}
// Also use the Inbuti function like Decimal() Int32() String() Bool()

// write a function to find the sum of digits of a number using loop

TEST_F(Function, SumOfDigitsOfANumberUsingLoopWhile) {
  std::string input = R"(
fun sumOfDigits(n) -> int {
  var sum = 0
  while (n != 0) {
    sum = sum + (n % 10)
    n = n // 10
  }
  return sum
}

print(sumOfDigits(123))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "6";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

#endif // NOT_JIT_TEST_MODE

#ifdef JIT_TEST_MODE

TEST_F(Function, NthgReturnTypeWithIntReturnValue2) {
  std::string input = R"(
fun main()-> nthg {
    return 2
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      "Function return type is Nothing, return expression is found";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);

// check expected output is substring of the output or not
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}
TEST_F(Function, NthgReturnTypeWithStringReturnValue2) {
  std::string input = R"(
    fun main()-> nthg {
      return "Hello"
    }
    print(main())
  )";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Function return type is Nothing, return expression is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, NthgReturnTypeWithBoolReturnValue2) {
  std::string input = R"(fun main()-> nthg {
    return (true)
}
print(main()))";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Function return type is Nothing, return expression is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}
TEST_F(Function, NthgReturnTypeWithDeciReturnValue2) {
  std::string input = R"(fun main()-> nthg {
    return 2.2
}
print(main()))";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Function return type is Nothing, return expression is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

// Int return type with int str bool deci nthg as return value

TEST_F(Function, IntReturnTypeWithNoReturnValue2) {
  std::string input = R"(fun main()-> int {
    return 2+2
}
print(main()))";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::string expected_output = "4";

  EXPECT_EQ(lowerCaseOutput, expected_output);
}

TEST_F(Function, IntReturnTypeWithNoReturnValueWithBrackets2) {
  std::string input = R"(fun main()-> int {
    return (2+2)
}
print(main()))";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::string expected_output = "4";

  EXPECT_EQ(lowerCaseOutput, expected_output);
}

TEST_F(Function, IntReturnTypeWithNthg2) {
  std::string input = R"(
fun main()-> int {
    return:
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Function return type is not Nothing, return expression is not found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}
TEST_F(Function, IntReturnTypeWithNoReturn2) {
  std::string input = R"(fun main2()-> int {
}print(main2()))";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Function return type is not Nothing, return expression is not found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, IntReturnTypeWithStrReturnValue2) {
  std::string input = R"(
fun main()-> int {
return "str"
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Return Type Mismatch Integer is expected but String is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, IntReturnTypeWithBoolReturnValue2) {
  std::string input = R"(
fun main()-> int {
return true
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Return Type Mismatch Integer is expected but Boolean is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);

#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, IntReturnTypeWithBoolReturnValueFalse2) {
  std::string input = R"(
fun main()-> int {
return false
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Return Type Mismatch Integer is expected but Boolean is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}
TEST_F(Function, IntReturnTypeWithDeciReturnValue2) {
  std::string input = R"(
fun main()-> int {
return 1.2
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Return Type Mismatch Integer is expected but Decimal is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

// Bool Return Type

TEST_F(Function, BoolReturnTypeWithIntReturnValue2) {
  std::string input = R"(
fun main()-> bool {
return 1
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Return Type Mismatch Boolean is expected but Integer is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, BoolReturnTypeWithNthgReturnValue2) {
  std::string input = R"(
fun main()-> bool {
return:
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Function return type is not Nothing, return expression is not found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, BoolReturnTypeWithNoReturn2) {
  std::string input = R"(
fun main()-> bool {

}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Function return type is not Nothing, return expression is not found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, BoolReturnTypeWithStringReturnValue2) {
  std::string input = R"(
fun main()-> bool {
return "str"
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Return Type Mismatch Boolean is expected but String is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, BoolReturnTypeWithBoolReturnValueTrue2) {
  std::string input = R"(
fun main()-> bool {
return true
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::string expected_output = R"(true)";

  EXPECT_EQ(lowerCaseOutput, expected_output);
}

TEST_F(Function, BoolReturnTypeWithBoolReturnValueFalse2) {
  std::string input = R"(
fun main()-> bool {
return false
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::string expected_output = R"(false)";

  EXPECT_EQ(lowerCaseOutput, expected_output);
}

TEST_F(Function, BoolReturnTypeWithDeciReturnValue2) {
  std::string input = R"(
fun main()-> bool {
return 1.2
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Return Type Mismatch Boolean is expected but Decimal is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

// String Return Type

TEST_F(Function, StringReturnTypeWithIntReturnValue2) {
  std::string input = R"(
fun main()-> str {
return 1
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Return Type Mismatch String is expected but Integer is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, StringReturnTypeWithNthgReturnValue2) {
  std::string input = R"(
fun main()-> str {
return:
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Function return type is not Nothing, return expression is not found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, StringReturnTypeWithNoReturnValue2) {
  std::string input = R"(
fun main()-> str {

}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Function return type is not Nothing, return expression is not found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, StringReturnTypeWithBoolReturnValueTrue2) {
  std::string input = R"(
fun main()-> str {
return true
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Return Type Mismatch String is expected but Boolean is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, StringReturnTypeWithBoolReturnValueFalse2) {
  std::string input = R"(
fun main()-> str {
return (false)
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Return Type Mismatch String is expected but Boolean is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, StringReturnTypeWithDeciReturnValue2) {
  std::string input = R"(
fun main()-> str {
return 1.2
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::transform(lowerCaseOutput.begin(), lowerCaseOutput.end(),
                 lowerCaseOutput.begin(), ::tolower);

  std::string expected_output =
      R"(Return Type Mismatch String is expected but Decimal is found)";
  std::transform(expected_output.begin(), expected_output.end(),
                 expected_output.begin(), ::tolower);
#ifdef JIT_TEST_MODE
  EXPECT_TRUE(Utils::isSubstring(lowerCaseOutput, expected_output));
#elif REPL_TEST_MODE
  EXPECT_TRUE(lowerCaseOutput.find(expected_output) != std::string::npos);
#endif
}

TEST_F(Function, NthgReturnTypeWithNthgReturnValue2) {
  std::string input = R"(
fun main()-> nthg {
    return:
}
main()
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();
  std::string expected_output;
#ifdef JIT_TEST_MODE
  expected_output = "";
#elif REPL_TEST_MODE
  expected_output = "";
#endif

  EXPECT_EQ(lowerCaseOutput, expected_output);
}
TEST_F(Function, NthgReturnTypeWithNoReturnValue2) {
  std::string input = R"(fun main()-> nthg {
  
}
main())";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::string expected_output;
#ifdef JIT_TEST_MODE
  expected_output = "";
#elif REPL_TEST_MODE
  expected_output = "";
#endif

  EXPECT_EQ(lowerCaseOutput, expected_output);
}

TEST_F(Function, StringReturnTypeWithStringReturnValue2) {
  std::string input = R"(
fun main()-> str {
return "Hello" + " World"
}
print(main())
)";

  setInput(input);
  runEvaluator();
  // Expected output should be in the of the output
  std::string lowerCaseOutput = getOutput();

  std::string expected_output = R"(Hello World)";

  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// sum of n numbers using recursion

TEST_F(Function, SumOfNNumbersUsingRecursion2) {
  std::string input = R"(
fun sum(n:int) -> int {
  if (n == 0) {
    return 0
  }
  return n + sum(n - 1)
}
print(sum(10))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "55";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// sum of n numbers using loop

TEST_F(Function, SumOfNNumbersUsingLoop2) {
  std::string input = R"(
fun sum(n:int) -> int {
  var sum:int = 0
  for (var i = 0 to n : 1) {
    sum = sum + i
  }
  return sum
}
print(sum(10))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "55";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// write a function to find the factorial of a number

TEST_F(Function, FactorialOfANumberUsingRecursion2) {
  std::string input = R"(
fun factorial(n:int) -> int {
  if (n == 0) {
    return 1
  }
  return n * factorial(n - 1)
}
print(factorial(5))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "120";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// write a function to find the factorial of a number using loop

TEST_F(Function, FactorialOfANumberUsingLoop2) {
  std::string input = R"(
fun factorial(n:int) -> int {
  var fact = 1
  for (var i = 1 to n : 1) {
    fact = fact * i
  }
  return fact
}
print(factorial(5))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "120";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// write a function to find the fibonacci of a number

TEST_F(Function, FibonacciOfANumberUsingRecursion2) {
  std::string input = R"(
fun fibonacci(n:int) -> int {
  if (n == 0) {
    return 0
  }
  if (n == 1) {
    return 1
  }
  return fibonacci(n - 1) + fibonacci(n - 2)
}
print(fibonacci(10))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "55";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// write a function to find the fibonacci of a number using loop

TEST_F(Function, FibonacciOfANumberUsingLoop2) {
  std::string input = R"(
fun fibonacci(n:int) -> int {
  var a = 0
  var b = 1
  var c = 0
  for (var i = 2 to n : 1) {
    c = a + b
    a = b
    b = c
  }
  return c
}
print(fibonacci(10))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "55";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// write a function to find the gcd of two numbers

TEST_F(Function, GCDOfTwoNumbersUsingRecuhrsion2) {
  std::string input = R"(
fun gcd(a:int, b:int) -> int {
  if (b == 0) {
    return a
  }
  return gcd(b, a % b)
}
print(gcd(10, 15))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "5";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// write a function to find the gcd of two numbers using loop

TEST_F(Function, GCDOfTwoNumbersUsingLoop2) {
  std::string input = R"(
fun gcd(a:int, b:int) -> int {
  while (b != 0) {
    var temp = b
    b = a % b
    a = temp
  }
  return a
}
print(gcd(10, 15))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "5";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// write a function to find the lcm of two numbers

TEST_F(Function, LCMOfTwoNumbersUsingRecursion2) {
  std::string input = R"(
fun gcd(a:int, b:int) -> int {
  if (b == 0) {
    return a
  }
  return gcd(b, a % b)
}
fun lcm(a:int, b:int) -> int {
  return ((a * b) // gcd(a, b))
}
print(lcm(10, 15))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "30";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// write a function to find the lcm of two numbers using loop

TEST_F(Function, LCMOfTwoNumbersUsingLoop2) {
  std::string input = R"(
fun gcd(a:int, b:int) -> int {
  while (b != 0) {
    var temp = b
    b = a % b
    a = temp
  }
  return a
}

fun lcm(a:int, b:int) -> int {
  return ((a * b) // gcd(a, b))
}

print(lcm(10, 15))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "30";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// write a function to find the power of a number

TEST_F(Function, PowerOfANumberUsingRecursion2) {
  std::string input = R"(
fun power(base:int, exp:int) -> int {
  if (exp == 0) {
    return 1
  }
  return base * power(base, exp - 1)
}

print(power(2, 3))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "8";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// write a function to find the power of a number using loop

TEST_F(Function, PowerOfANumberUsingLoop2) {
  std::string input = R"(
fun power(base:int, exp:int) -> int {
  var result = 1
  for (var i = 0 to exp - 1: 1) {
    result = result * base
  }
  return result
}

print(power(2, 3))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "8";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// write a function to find the sum of digits of a number

TEST_F(Function, SumOfDigitsOfANumberUsingRecursion2) {
  std::string input = R"(

fun sumOfDigits(n:int) -> int {
  if (n == 0) {
    return 0
  }
  return ((n % 10) + sumOfDigits(n // 10))
}

print(sumOfDigits(123))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "6";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// write a function to find the sum of digits of a number using loop

TEST_F(Function, SumOfDigitsOfANumberUsingLoop2) {
  std::string input = R"(
fun sumOfDigits(n:int) -> int {
  var sum = 0
  while (n != 0) {
    sum = sum + (n % 10)
    n = n // 10
  }
  return sum
}

print(sumOfDigits(123))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "6";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

TEST_F(Function, SumOfDigitsOfANumberUsingLoopWhile2) {
  std::string input = R"(
fun sumOfDigits(n:int) -> int {
  var sum:int = 0
  while (n != 0) {
    sum = sum + (n % 10)
    n = n // 10
  }
  return sum
}

print(sumOfDigits(123))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "6";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

TEST_F(Function, FunctionStringTest) {
  std::string input = R"(

fun getConactString(b:str,a:str)-> str {

    return a + b
}

print(getConactString("Hello","World"))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "WorldHello";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

TEST_F(Function, FunctionIntTest) {
  std::string input = R"(
fun getSum(a:int,b:int)-> int {
    return a + b
}

print(getSum(2,3))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "5";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

TEST_F(Function, FunctionBoolTest) {
  std::string input = R"(
fun getBool(a:bool,b:bool)-> bool {
    return a && b
}

print(getBool(true,false))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "false";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

TEST_F(Function, FunctionDeciTest) {
  std::string input = R"(

fun getSum(a:deci,b:deci)-> deci {
    return a + b
}

print(getSum(2.2,3.3))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "5.50000000000000";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// TODO: Nthg Tests are Pending

// TEST_F(Function, FunctionNthgTest) {
//   std::string input = R"(
// fun getNthg(a:nthg,b:nthg)-> nthg {
//     return:
// }

// print(getNthg(2,null))
// )";
//   setInput(input);
//   runEvaluator();
//   std::string lowerCaseOutput = getOutput();
//   std::string expected_output = "";
//   EXPECT_EQ(lowerCaseOutput, expected_output);
// }

TEST_F(Function, FunctionIntWithDeciTest) {
  std::string input = R"(
fun getSum(a:int,b:deci)-> deci {
    return a + b

}

print(getSum(2,3.3))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "5.30000000000000";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

TEST_F(Function, FunctionIntWithBoolTest) {
  std::string input = R"(
fun getSum(a:int,b:bool)-> int {
    return a + b

}

print(getSum(2,true))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "3";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

TEST_F(Function, FunctionIntWithStringTest) {
  std::string input = R"(
fun getSum(a:int,b:str)-> str {
    return a + b

}

print(getSum(2,"Hello"))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "2Hello";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// TEST_F(Function, FunctionIntWithNthgTest) {
//   std::string input = R"(

// fun getSum(a:int,b:nthg)-> int {
//     return a + b

// }

// print(getSum(2,3))
// )";
//   setInput(input);
//   runEvaluator();
//   std::string lowerCaseOutput = getOutput();
//   std::string expected_output = "5";
//   EXPECT_EQ(lowerCaseOutput, expected_output);
// }

TEST_F(Function, FunctionDeciWithIntTest) {
  std::string input = R"(
fun getSum(a:deci,b:int)-> deci {
    return a + b

}

print(getSum(2.2,3))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "5.20000000000000";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

TEST_F(Function, FunctionDeciWithDeciTest) {
  std::string input = R"(

fun getSum(a:deci,b:deci)-> deci {
    return a + b

}

print(getSum(2.2,3.3))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "5.50000000000000";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

TEST_F(Function, FunctionDeciWithBoolTest) {
  std::string input = R"(

fun getSum(a:deci,b:bool)-> deci {
    return a + b

}

print(getSum(2.2,true))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "3.20000000000000";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

TEST_F(Function, FunctionDeciWithStringTest) {
  std::string input = R"(

fun getSum(a:deci,b:str)-> str {
    return a + b

}

print(getSum(2.2,"Hello"))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "2.20000000000000Hello";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

// TEST_F(Function, FunctionDeciWithNthgTest) {
//   std::string input = R"(

// fun getSum(a:deci,b:nthg)-> deci {
//     return a + b

// }

// print(getSum(2.2,3))
// )";
//   setInput(input);
//   runEvaluator();
//   std::string lowerCaseOutput = getOutput();
//   std::string expected_output = "5.200000";
//   EXPECT_EQ(lowerCaseOutput, expected_output);
// }

TEST_F(Function, FunctionBoolWithIntTest) {
  std::string input = R"(

fun getSum(a:bool,b:int)-> int {
    return a + b

}

print(getSum(true,3))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "4";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

TEST_F(Function, FunctionBoolWithDeciTest) {
  std::string input = R"(

fun getSum(a:bool,b:deci)-> deci {
    return a + b

}

print(getSum(true,3.3))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "4.30000000000000";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

TEST_F(Function, FunctionBoolWithBoolTest) {
  std::string input = R"(

fun getSum(a:bool,b:bool)-> bool {
    return a && b

}

print(getSum(true,false))
)";
  setInput(input);
  runEvaluator();
  std::string lowerCaseOutput = getOutput();
  std::string expected_output = "false";
  EXPECT_EQ(lowerCaseOutput, expected_output);
}

#endif
