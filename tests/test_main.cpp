#include "../src/REPL/Repl.h"
#include "../src/lexer/Lexer.h"
#include <gtest/gtest.h>
#include <sstream>

// Test fixture for I/O redirection
class IORedirectionTest : public ::testing::Test {
protected:
  std::stringstream input_stream;
  std::stringstream output_stream;
  std::streambuf *saved_cout_buf;

  Repl *repl = nullptr;
  void SetUp() override {
    // Redirect cout to the stringstream

    saved_cout_buf = std::cout.rdbuf(output_stream.rdbuf());
  }

  void TearDown() override {
    // Restore cout back to the standard output
    std::cout.rdbuf(saved_cout_buf);
  }

  void setInput(const std::string &input) { input_stream.str(input); }

  std::string getOutput() const { return output_stream.str(); }

  void runEvaluator() {
    repl = new Repl();

    repl->runForTest(input_stream, output_stream);
  }
};

// Test case for Repl::handleSpecialCommands

TEST_F(IORedirectionTest, TestHandleSpecialCommands) {
  setInput(":tree");
  runEvaluator();
  ASSERT_TRUE(repl->isTreeVisible());
}

// Test case for Repl::countBraces

TEST_F(IORedirectionTest, TestCountBraces) {
  ASSERT_EQ(repl->countBraces("{}", '{'), 1);
  ASSERT_EQ(repl->countBraces("{}", '}'), 1);
  ASSERT_EQ(repl->countBraces("{{}}", '{'), 2);
  ASSERT_EQ(repl->countBraces("{{}}", '}'), 2);
}

// Test case for Repl::processInput

TEST_F(IORedirectionTest, TestProcessInput) {
  setInput("1 + 2");
  runEvaluator();
  ASSERT_EQ(getOutput(), "3\n");
}

// Test case for Repl::compileAndEvaluate

TEST_F(IORedirectionTest, TestCompileAndEvaluate) {
  setInput("1 + 2");
  runEvaluator();
  ASSERT_EQ(getOutput(), "3\n");
}

// Test case for Repl::printWelcomeMessage

TEST_F(IORedirectionTest, TestPrintWelcomeMessage) {
  runEvaluator();
  ASSERT_EQ(getOutput(), "");
}

// Test case for evaluating a single expression

TEST_F(IORedirectionTest, TestSingleExpression) {
  setInput("1 + 2");
  runEvaluator();
  ASSERT_EQ(getOutput(), "3\n");
}

// Test case for evaluating multiple expressions

TEST_F(IORedirectionTest, TestMultipleExpressions) {
  setInput("1 + 2\n3 + 4");
  runEvaluator();
  ASSERT_EQ(getOutput(), "3\n7\n");
}

// Test case for evaluating a single expression with a variable

TEST_F(IORedirectionTest, TestSingleExpressionWithVariable) {
  setInput("var x = 1 + 2");
  runEvaluator();
  ASSERT_EQ(getOutput(), "3\n");
}

// Test case for executing a single expression with a different type

TEST_F(IORedirectionTest, TestSingleExpressionWithDifferentType) {
  setInput("var x = 1 + 2");
  runEvaluator();
  ASSERT_EQ(getOutput(), "3\n");
}

// Test case for executing a single expression with a different type

TEST_F(IORedirectionTest, TestSingleExpressionWithDifferentType2) {
  setInput("var x = 1 + 2.0");
  runEvaluator();
  ASSERT_EQ(getOutput(), "3\n");
}

// Test case for executing a single expression with a commbianation of types and
// expressions

TEST_F(IORedirectionTest, TestSingleExpressionWithDifferentType3) {
  setInput("var y = 3 * true");
  runEvaluator();
  ASSERT_EQ(getOutput(), "3\n");
}

// Test case for executing a single expression with a commbianation of types

TEST_F(IORedirectionTest, TestSingleExpressionWithDifferentType4) {
  setInput("var y = 3 * false");
  runEvaluator();
  ASSERT_EQ(getOutput(), "0\n");
}

// Test case for executing a single expression with a commbianation of types

TEST_F(IORedirectionTest, TestSingleExpressionWithDifferentType5) {
  setInput("var y = 3 * \"Hello\"");
  runEvaluator();
  ASSERT_EQ(getOutput(),
            "\x1B[31mError: Multiplication not supported for string\x1B[0m\n");
}

// Test case for executing a single expression with a commbianation of  double &
// int

TEST_F(IORedirectionTest, TestSingleExpressionWithDifferentType6) {
  setInput("var y = 3 * 2.0");
  runEvaluator();
  ASSERT_EQ(getOutput(), "6\n");
}

// Test case for executing a single expression with a commbianation of  double &
// int

TEST_F(IORedirectionTest, TestSingleExpressionWithDifferentType7) {
  setInput("var y = 3.1 * 2");
  runEvaluator();
  ASSERT_EQ(getOutput(), "6.2\n");
}

// Test case for executing a single expression with a commbianation of bool &
// double

TEST_F(IORedirectionTest, TestSingleExpressionWithDifferentType8) {
  setInput("var y = 3.1 * true");
  runEvaluator();
  ASSERT_EQ(getOutput(),
            "\x1B[31mError: Multiplication not supported for double\x1B[0m\n");
}

// Test case for executing a single expression with a commbianation of string &
// double

TEST_F(IORedirectionTest, TestSingleExpressionWithDifferentType9) {
  setInput("var y = 3.1 + \"Hello\"");
  runEvaluator();
  ASSERT_EQ(getOutput(), "3.100000Hello\n");
}

// Test case for executing a single expression with a commbianation of string &
// bool

TEST_F(IORedirectionTest, TestSingleExpressionWithDifferentType10) {
  setInput("var y = true + \"Hello\"");
  runEvaluator();
  ASSERT_EQ(getOutput(),
            "\x1B[31mError: Addition not supported for string\x1B[0m\n");
}

// Test Case for String concatenation

TEST_F(IORedirectionTest, TestStringConcatenation) {
  setInput("var y = \"Hello\" + \"World\"");
  runEvaluator();
  ASSERT_EQ(getOutput(), "HelloWorld\n");
}

// Test Case for String  concatenation with integer

TEST_F(IORedirectionTest, TestStringConcatenationWithInteger) {
  setInput("var y = \"Hello\" + 1");
  runEvaluator();
  ASSERT_EQ(getOutput(), "Hello1\n");
}

// Test Case for String  concatenation with double

TEST_F(IORedirectionTest, TestStringConcatenationWithDouble) {
  setInput("var y = \"Hello\" + 1.0");
  runEvaluator();
  ASSERT_EQ(getOutput(), "Hello1.000000\n");
}

// Test Case for String  concatenation with bool

TEST_F(IORedirectionTest, TestStringConcatenationWithBool) {
  setInput("var y = \"Hello\" + true");
  runEvaluator();
  ASSERT_EQ(getOutput(),
            "\x1B[31mError: Addition not supported for string\x1B[0m\n");
}

// Bitwise operators

// Test case for executing a single expression with a commbianation of string &

TEST_F(IORedirectionTest, TestSingleExpressionWithDifferentType11) {
  setInput("var y = 3 & 2");
  runEvaluator();
  ASSERT_EQ(getOutput(), "2\n");
}

// Test case for executing a single expression with a commbianation of string &
// bool

TEST_F(IORedirectionTest, TestSingleExpressionWithDifferentType12) {
  setInput("var y = 3 & true");
  runEvaluator();
  ASSERT_EQ(getOutput(), "1\n");
}

// Test case for executing a single expression with a commbianation of string ,
// dobule , bool & int in same expression

TEST_F(IORedirectionTest, TestSingleExpressionWithDifferentType14) {
  setInput("var y = 3 & false & true");
  runEvaluator();
  ASSERT_EQ(getOutput(), "0\n");
}

/*
    Statements
*/
// Test Case for evaluating a single statement

TEST_F(IORedirectionTest, TestSingleStatement) {
  setInput("{var x = 1 + 2}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "3\n");
}

// Test Case for evaluating multiple statements

TEST_F(IORedirectionTest, TestMultipleStatements) {
  setInput("{var x = 1 + 2\nvar y = 3 + 4}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "7\n");
}

// Test Case for evaluating a single statement with a variable

TEST_F(IORedirectionTest, TestSingleStatementWithVariable) {
  setInput("{var x = 1 + 2\nx}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "3\n");
}

// Test Case for evaluating a single statement with a variable

TEST_F(IORedirectionTest, TestSingleStatementWithVariable2) {
  setInput("{var x = 1 + 2\nvar y = x + 3\ny}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "6\n");
}

// Test Case for evaluating a single statement with a variable

TEST_F(IORedirectionTest, TestSingleStatementWithVariable3) {
  setInput("{var x = 1 + 2\nvar y = x + 3\nvar z = y + 4\nz}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "10\n");
}

// Test Case for evaluating a single statement with a variable

TEST_F(IORedirectionTest, TestSingleStatementWithVariable4) {
  setInput("{var x = 1 + 2\nvar y = x + 3\nvar z = y + 4\nvar a = z + 5\na}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "15\n");
}

// Test Case for evaluating a Multiple statement with a variable

TEST_F(IORedirectionTest, TestMultipleStatementWithVariable) {
  setInput("{var x = 1 + 2\nvar y = x + 3\nvar z = y + 4\nvar a = z + "
           "5\na\nx\ny\nz}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "10\n");
}

// Test Case for evaluating a If statement with a variable

TEST_F(IORedirectionTest, TestIfStatementWithVariable) {
  setInput("{var x = 1 + 2\nvar y = x + 3\nvar z = y + 4\nvar a = z + "
           "5\nif(a == 15) {a}\nelse {x}\n}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "15\n");
}

// Test Case for evaluating a If statement  with boolean

TEST_F(IORedirectionTest, TestIfStatementWithBoolean) {
  setInput("{var x = 1 + 2\nvar y = x + 3\nvar z = y + 4\nvar a = z + "
           "5\nif(a == 15) {true}\nelse {false}\n}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "true\n");
}

// Test Case for evaluating a If statement  with  integer

TEST_F(IORedirectionTest, TestIfStatementWithInteger) {
  setInput("{var x = 1 + 2\nvar y = x + 3\nvar z = y + 4\nvar a = z + "
           "5\nif(a == 15) {1}\nelse {0}\n}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "1\n");
}

/*
    BlockStatements
*/
// Test Case for evaluating a Block statement  with  integer

TEST_F(IORedirectionTest, TestBlockStatementWithInteger) {
  setInput("{var x = 1 + 2\nvar y = x + 3\nvar z = y + 4\nvar a = z + "
           "5\nif(a == 15) {1}\nelse {0}\n}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "1\n");
}

// Test Case for evaluating a Nested Block statement  with  integer

TEST_F(IORedirectionTest, TestNestedBlockStatementWithInteger) {
  setInput("{var x = 1 + 2\nvar y = x + 3\nvar z = y + 4\nvar a = z + "
           "5\nif(a == 15) {1}\nelse {0}\n}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "1\n");
}

// Test Case for evaluating a Nested Block statement  with  different types

TEST_F(IORedirectionTest, TestNestedBlockStatementWithDifferentTypes) {
  setInput("{var x = 1 + 2\nvar y = x + 3\nvar z = y + 4\nvar a = z + "
           "5\nif(a == 15) {1}\nelse {0}\n}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "1\n");
}

// Test Case for evaluating a Nested Block statement  with  different data types
// & expressions

TEST_F(IORedirectionTest, TestNestedBlockStatementWithDifferentTypes2) {
  setInput("{var x = 1 + 2\nvar y = x + 3\nvar z = y + 4\nvar a = z + "
           "5\nif(a == 15) {1}\nelse {0}\n}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "1\n");
}

// Test Case for const statement with  different data types & expressions

TEST_F(IORedirectionTest, TestConstStatementWithDifferentTypes) {
  setInput("{const x = 1 + 2\nconst y = x + 3\nconst z = y + 4\nconst a = z + "
           "5\nif(a == 15) {1}\nelse {0}\n}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "1\n");
}

// Test Case for const variable reassignment

TEST_F(IORedirectionTest, TestConstVariableReassignment) {
  setInput("{const x = 1 + 2\nx = 3\n}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "\x1B[31mError: Variable x is const\x1B[0m\n");
}

// Test Case for const variable declaration

TEST_F(IORedirectionTest, TestConstVariableDeclaration) {
  setInput("{const x = 1 + 2\nvar x = 3\n}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "\x1B[31mError: Variable x already exists\x1B[0m\n");
}

// Test Case for var variable reassignment

TEST_F(IORedirectionTest, TestVarVariableReassignment) {
  setInput("{var x = 1 + 2\nx = 3\n}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "3\n");
}

// Test Case for var variable redeclaration in same scope

TEST_F(IORedirectionTest, TestVarVariableRedeclaration) {
  setInput("{var x = 1 + 2\nvar x = 3\n}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "\x1B[31mError: Variable x already exists\x1B[0m\n");
}

// Test Case for var variable redeclaration in different scope

TEST_F(IORedirectionTest, TestVarVariableRedeclaration2) {
  setInput("{var x = 1 + 2\n{\nvar x = 3\n}\n}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "3\n");
}

// Test Case for If statement with  different data types & expressions

TEST_F(IORedirectionTest, TestIfStatementWithDifferentTypes) {
  setInput("{const x = 1 + 2\nconst y = x + 3\nconst z = y + 4\nconst a = z + "
           "5\nif(a == 15) {1}\nelse {0}\n}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "1\n");
}

// Test Case for while statement with  different data types & expressions

TEST_F(IORedirectionTest, TestWhileStatementWithDifferentTypes) {
  setInput("{const x = 1 + 2\nconst y = x + 3\nconst z = y + 4\nconst a = z + "
           "5\nwhile(a == 12) {1} if a==15 {a=a+1} \n}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "16\n");
}

// Test Case for Nested while statement with  different data types & expressions

TEST_F(IORedirectionTest, TestNestedWhileStatementWithDifferentTypes) {
  setInput("{const x = 1 + 2\nconst y = x + 3\nconst z = y + 4\nconst a = z + "
           "5\nwhile(a == 13) {1\nwhile(a == 12) {1}\n}\n}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "15\n");
}

// Test Case for Nested while statement condition with  different int data types

TEST_F(IORedirectionTest, TestNestedWhileStatementConditionWithDifferentTypes) {
  setInput("{const x = 1 + 2\nconst y = x + 3\nconst z = y + 4\nconst a = z + "
           "5\nwhile(!a) {1\nwhile(a == 15) {1}\n}\n}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "15\n");
}

// Sum of first 10 numbers

TEST_F(IORedirectionTest, TestSumOfFirst10Numbers) {
  setInput("{var x = 0\nvar y = 0\nwhile(x < 10) {y = y + x\nx = x + 1}\ny}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "45\n");
}

// multiplication of first 10 numbers in while loop where condition is true

TEST_F(IORedirectionTest, TestMultiplicationOfFirst10Numbers) {
  setInput("{var x = 1\nvar y = 1\nwhile(x < 10) {y = y * x\nx = x + 1}\ny}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "362880\n");
}

// complex while loop

TEST_F(IORedirectionTest, TestComplexWhileLoop) {
  setInput("{var x = 1\nvar y = 1\nwhile(x < 10) {y = y * x\nx = x + 1}\ny}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "362880\n");
}

// Complex multiple while loop with fizzbuzz  problem

TEST_F(IORedirectionTest, TestComplexMultipleWhileLoop) {
  setInput("{var x = 1\nvar y = 1\nwhile(x < 10) {y = y * x\nx = x + 1}\nvar "
           "z=1\nwhile(z<100){if(z%3==0){if(z%5==0){z=z+1}\nelse{z=z+1}}\nelse "
           "{if(z%5==0){z=z+1}\nelse{z=z+1}}}\n}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "100\n");
}

// Test for for loop

TEST_F(IORedirectionTest, TestForLoop) {
  setInput("{var x = 0\nfor(var i = 0 to 9) {x = x + i}\nx}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "45\n");
}

// Test for for loop without perenthesis

TEST_F(IORedirectionTest, TestForLoopWithoutPerenthesis) {
  setInput("{var x = 0\nfor var i = 0 to 9 {x = x + i}\nx}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "45\n");
}

// Test for for loop and variable redeclaration

TEST_F(IORedirectionTest, TestForLoopAndVariableRedeclaration) {
  setInput("{var x = 0\nfor var i = 0 to 9 {var x = 1\nx = x + i}\nx}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "10\n");
}

// Test for for loop and variable assignment

TEST_F(IORedirectionTest, TestForLoopAndVariableAssignmentInCondition) {
  setInput("{var x = 0\nfor var i = 0 to x {var x = 1\nx = x + i}\nx}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "1\n");
}

// Test for for loop and variable assignment in condition

TEST_F(IORedirectionTest, TestForLoopAndVariableAssignmentInCondition2) {
  setInput("{var x = 10\nfor x to 20 {}\nx}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "10\n");
}

TEST_F(IORedirectionTest,
       TestForLoopAndVariableAssignmentInConditionNoVariableDeclaration) {
  setInput("{var x = 10\nfor 2 to 5 {x=x+1}\nx}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "14\n");
}

TEST_F(IORedirectionTest,
       TestForLoopAndVariableAssignmentInConditionWithVariableDeclaration) {
  setInput("{var x = 20\nfor var y = 0 to 3 {x=x+1}\nx}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "24\n");
}

// Test For Conversion  Int32 to String

TEST_F(IORedirectionTest, TestForConversionInt32ToString) {
  setInput("String(10)");
  runEvaluator();
  ASSERT_EQ(getOutput(), "10\n");
}

// Test For Conversion  Double to String

TEST_F(IORedirectionTest, TestForConversionDoubleToString) {
  setInput("String(10.0)");
  runEvaluator();
  ASSERT_EQ(getOutput(), "10.000000\n");
}

// Test For Conversion  Bool to String

TEST_F(IORedirectionTest, TestForConversionBoolToString) {
  setInput("String(true)");
  runEvaluator();
  ASSERT_EQ(getOutput(), "1\n");
}

// Test For Conversion  String to Int32

TEST_F(IORedirectionTest, TestForConversionStringToInt32) {
  setInput("Int32(\"10\")");
  runEvaluator();
  ASSERT_EQ(getOutput(), "10\n");
}

// Test For Conversion  Double to Int32

TEST_F(IORedirectionTest, TestForConversionDoubleToInt32) {
  setInput("Int32(10.0)");
  runEvaluator();
  ASSERT_EQ(getOutput(), "10\n");
}

// Test For Conversion  Bool to Int32

TEST_F(IORedirectionTest, TestForConversionBoolToInt32) {
  setInput("Int32(true)");
  runEvaluator();
  ASSERT_EQ(getOutput(), "1\n");
}

// Test For Conversion  String to Double

TEST_F(IORedirectionTest, TestForConversionStringToDouble) {
  setInput("Double(\"10.1\")");
  runEvaluator();
  ASSERT_EQ(getOutput(), "10.1\n");
}

// Test For Conversion  Int32 to Double

TEST_F(IORedirectionTest, TestForConversionInt32ToDouble) {
  setInput("Double(10)");
  runEvaluator();
  ASSERT_EQ(getOutput(), "10\n");
}

// Test For Conversion  Bool to Double

TEST_F(IORedirectionTest, TestForConversionBoolToDouble) {
  setInput("Double(true)");
  runEvaluator();
  ASSERT_EQ(getOutput(), "1\n");
}

// Test For Conversion  Int32 to Bool

TEST_F(IORedirectionTest, TestForConversionInt32ToBool) {
  setInput("Bool(10)");
  runEvaluator();
  ASSERT_EQ(getOutput(), "true\n");
}

// Test For Conversion  Double to Bool

TEST_F(IORedirectionTest, TestForConversionDoubleToBool) {
  setInput("Bool(10.0)");
  runEvaluator();
  ASSERT_EQ(getOutput(), "true\n");
}

// Test For Conversion  String to Bool

TEST_F(IORedirectionTest, TestForConversionStringToBool) {
  setInput("Bool(\"10\")");
  runEvaluator();
  ASSERT_EQ(getOutput(), "true\n");
}

// Test for inbuilt function print

TEST_F(IORedirectionTest, TestForInbuiltFunctionPrint) {
  setInput("{var x =2 var y =x x=x+1 y = Int32(y) y=y+1 print(\"hello\" + x "
           "+\" hii \"+y)}");
  runEvaluator();
  ASSERT_EQ(getOutput(), "hello3 hii 3");
}

// Test For Conversion  String to String

// Test fixture for Lexer class
class LexerTest : public ::testing::Test {
protected:
  Lexer lexer;

public:
  void setInput(const std::string &input) {
    lexer = Lexer(std::vector<std::string>{input});
  }

  void setInput(const std::vector<std::string> &input) { lexer = Lexer(input); }

  std::shared_ptr<SyntaxToken<std::any>> readString() {
    return lexer.readString();
  }

  LexerTest() : lexer(std::vector<std::string>{}) {}
};

// Test case for valid string literals
// TEST_F(LexerTest, TestValidStringLiteral) {
//   std::string input = "Hello, world!";
//   setInput(input);
//   std::shared_ptr<SyntaxToken<std::any>>token =
//   LexerTest::lexer.readString();

//   ASSERT_EQ(token->getKind(), SyntaxKindUtils::SyntaxKind::StringToken);
//   ASSERT_EQ(std::any_cast<std::string>(token->getValue()), "Hello, world!");
// }

// Test case for unterminated string literals
// TEST_F(LexerTest, TestUnterminatedStringLiteral) {
//   std::string input = "Hello, world!";
//   setInput(input);
//   std::shared_ptr<SyntaxToken<std::any>>token = readString();

//   ASSERT_EQ(token->getKind(), SyntaxKindUtils::SyntaxKind::BadToken);
//   ASSERT_TRUE(token->getText().find("ERROR: unterminated string literal") !=
//               std::string::npos);
// }

// // Test case for invalid escape sequences
// TEST_F(LexerTest, TestInvalidEscapeSequence) {
//   std::string input = "Hello, \\world!";
//   setInput(input);
//   std::shared_ptr<SyntaxToken<std::any>>token = readString();

//   ASSERT_EQ(token->getKind(), SyntaxKindUtils::SyntaxKind::BadToken);
//   ASSERT_TRUE(token->getText().find("ERROR: bad character escape sequence")
//   !=
//               std::string::npos);
// }

// Add more test cases as needed to cover other scenarios

// Don't forget to deallocate memory to avoid memory leaks.
// TEST_F(LexerTest, TestMemoryDeallocation) {

//   std::string input = "Hello, world!";
//   setInput(input);
//   std::shared_ptr<SyntaxToken<std::any>>token = readString();
//   delete token;
// }

// Main function to run all tests
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}