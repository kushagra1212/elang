; ModuleID = 'ElangModule'
source_filename = "ElangModule"

declare i8* @malloc(i64)
declare i64 @strlen(i8*)
declare void @memcpy(i8*, i8*, i64, i1)

declare i32 @snprintf(i8*, i64, i8*, i32)
@intFormat = constant [3 x i8] c"%d\00"
define i8* @concat_strings(i8* %str1, i8* %str2) {
    ; Get the lengths of the input strings
    %len1 = call i64 @strlen(i8* %str1)
    %len2 = call i64 @strlen(i8* %str2)
    
    ; Allocate memory for the concatenated string
    %totalLen = add i64 %len1, %len2
    %concatStr = call i8* @malloc(i64 %totalLen)
    
    ; Copy characters from the first string to the concatenated string
    %ptr1 = bitcast i8* %concatStr to i8*
    call void @memcpy(i8* %ptr1, i8* %str1, i64 %len1, i1 false)
    
    ; Copy characters from the second string to the concatenated string
    %ptr2 = getelementptr i8, i8* %ptr1, i64 %len1
    call void @memcpy(i8* %ptr2, i8* %str2, i64 %len2, i1 false)

    ; Null-terminate the concatenated string
    %nullPtr = getelementptr i8, i8* %concatStr, i64 %totalLen
    store i8 0, i8* %nullPtr
    ret i8* %concatStr
}
declare i32 @puts(i8*)

define void @print_string(i8* %str) {
    call i32 @puts(i8* %str)
    ret void
}

define i8* @itos(i32 %num) {
    ; Allocate memory for the string buffer
    %buffer = call i8* @malloc(i64 12)
    
    ; Convert the integer to a string
    %formatStr = getelementptr [3 x i8], [3 x i8]* @intFormat, i32 0, i32 0
    call i32 @snprintf(i8* %buffer, i64 12, i8* %formatStr, i32 %num)

    ; Return the result as a pointer to the string
    ret i8* %buffer
}


@str1_constant = constant [8 x i8] c"Hello, \00"
@str2_constant = constant [7 x i8] c"world!\00"
define i32 @main() {
    ; Call concat_strings function
        %str1 = getelementptr [8 x i8], [8 x i8]* @str1_constant, i32 0, i32 0
    %str2 = getelementptr [7 x i8], [7 x i8]* @str2_constant, i32 0, i32 0
    
   %concatenated = call i8* @concat_strings(i8* %str1, i8* %str2)
    
    ; Print the concatenated string
    call void @print_string(i8* %concatenated)
    
    ; Convert an integer to a string using itos function
    %num = add i32 42, 8
    %num_str = call i8* @itos(i32 %num)
    
    ; Print the integer string
    call void @print_string(i8* %num_str)
    
    ret i32 0
}
