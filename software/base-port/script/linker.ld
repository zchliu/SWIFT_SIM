ENTRY(_start) /* 表示程序从 _start 函数开始执行 */

SECTIONS {
  /* _pmem_start and _entry_offset are defined in LDFLAGS */
  . = _pmem_start + _entry_offset; /* 设置当前地址为 _pmem_start + _entry_offset */
  .text : {     /* 定义代码段（text segment），包含所有代码 */
    *(entry)    /* 包含 entry 段中的所有内容，在反汇编 txt 文件中开头就是 */
    *(.text*)   /* 包含所有以 .text 开头的段 */
  } 
  etext = .; /* 定义 etext 和 _etext 符号，表示代码段的结束地址 */
  _etext = .;
  .rodata : {
    *(.rodata*) /* 包含所有 .rodata 开头的段 */
  }
  .data : {
    *(.data) /* 定义数据段，包含所有的初始化数据 */
  }
  edata = .;
  _data = .;
  .bss : {   /* 定义未初始化的数据段，包含所有未初始化的数据 */
	_bss_start = .;  
    *(.bss*)
    *(.sbss*)
    *(.scommon)
  }
  _stack_top = ALIGN(0x1000); /* 对齐栈顶地址到 0x1000 的边界 */
  . = _stack_top + 0x8000; /* 栈的大小为 0x8000 */
  _stack_pointer = .; /* 栈指针的初始位置 */
  end = .; /* 表示程序的结束符号 */
  _end = .;
  _heap_start = ALIGN(0x1000); /* 对齐堆的地址到 0x1000 边界，堆是从低地址向高地址增长 */
}
