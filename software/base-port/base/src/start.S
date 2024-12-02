.section entry, "ax" // 定义一个名为 entry 的段，ax 表示这个段是可执行 a 和包含代码 x
.globl _start // 这一行声明 _start 是一个全局符号，意味着可以被其他文件引用，通常 _start 是程序的入口点，链接器从这里开始执行
.type _start, @function // 这一行定义 _start 的类型是函数，有助于调试器和其他工具理解 _start 是一个可执行的代码段，而不是数据

_start:
  mv s0, zero
  la sp, _stack_pointer
  jal call_main

