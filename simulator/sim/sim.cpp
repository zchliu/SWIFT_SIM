#include "verilated_dpi.h"
#include "VCPU.h"
#include "verilated_vcd_c.h"
#include <bits/stdc++.h>
#include "include/debug.h"
#include "include/common.h"
#include "include/disasm.h"
#include "memory/paddr.h"

using namespace std;

extern VCPU *dut;
extern uint64_t sim_time;
extern VerilatedVcdC *m_trace;

extern uint8_t pmem[];

void print_itrace();
void print_cache_info();
void difftest_step();

// Lab2 HINT: instruction log struct for instruction trace
struct inst_log
{
  word_t pc;
  word_t inst;
};

class CircularQueue
{
private:
  inst_log *queue; // 队列数组，用于存储 inst_log 结构体
  int front, rear;
  int capacity;

public:
  CircularQueue(int cap)
  {
    capacity = cap;
    queue = new inst_log[capacity]; // 动态分配队列空间
    front = 0;
    rear = 0;
  }

  ~CircularQueue()
  {
    delete[] queue; // 释放队列内存
  }

  // 向队列中添加元素（pc 和 inst），自动覆盖最旧的元素
  void enqueue(word_t pc, word_t inst)
  {
    queue[rear].pc = pc;
    queue[rear].inst = inst;
    rear = (rear + 1) % capacity; // 更新 rear 指针
    if (rear == front)
    {
      front = (front + 1) % capacity; // 队列满时，更新 front 指针，覆盖最旧元素
    }
  }

  // 打印队列中的全部元素
  void display()
  {
      printf("Queue elements: pc, inst, assemble\n");
      int i = front;
      while (i != rear)
      {
          const int size = 32;
          char assemble_str[size];
          disassemble(assemble_str, size, queue[i].pc, (uint8_t *)&queue[i].inst, 4);
          if (i == (rear - 1 + capacity) % capacity) {
              printf("--> ");
          } else {
              printf("    ");
          }
          printf("(0x%08X, 0x%08X, %s)\n", queue[i].pc, queue[i].inst, assemble_str);

          i = (i + 1) % capacity;
      }
  }

  // 打印队列当中的后 n 个元素
  void display(unsigned int n) {
    printf("Queue elements: pc, inst, assemble\n");
    int count = (rear >= front) ? (rear - front) : (capacity - front + rear);
    int start = (count > n) ? (rear - n + capacity) % capacity : front;
    int max_elements = (n > count) ? count : n;
    int i = start;
    int printed = 0;

    while (printed < max_elements) {
        const int size = 32;
        char assemble_str[size];
        disassemble(assemble_str, size, queue[i].pc, (uint8_t *)&queue[i].inst, 4);
        if (printed == max_elements - 1) {
            printf("--> ");
        } else {
            printf("    ");
        }
        printf("(0x%08X, 0x%08X, %s)\n", queue[i].pc, queue[i].inst, assemble_str);

        i = (i + 1) % capacity;
        printed++;
    }
  }
};

CircularQueue q(16);

uint32_t *cpu_mstatus = NULL, *cpu_mtvec = NULL, *cpu_mepc = NULL, *cpu_mcause = NULL;
// load the state of your simulated cpu into sim_cpu
void set_state()
{
  sim_cpu.pc = dut->pc_cur;
  memcpy(&sim_cpu.gpr[0], cpu_gpr, 4 * 32);
  memcpy(&sim_cpu.csr.mepc, cpu_mepc, 4);
  memcpy(&sim_cpu.csr.mstatus, cpu_mstatus, 4);
  memcpy(&sim_cpu.csr.mcause, cpu_mcause, 4);
  memcpy(&sim_cpu.csr.mtvec, cpu_mtvec, 4);
}

// num of executed instruction
uint64_t g_nr_guest_inst = 0;

// simulate a single cycle
void single_cycle()
{
  dut->clk = 1;
  dut->eval();
  // if (g_nr_guest_inst > 490000)
    // m_trace->dump(sim_time++); 
#ifdef AXI
    pmem_write();
    pmem_read();
#endif
  dut->clk = 0;
  dut->eval();
  // if (g_nr_guest_inst > 490000)
    // m_trace->dump(sim_time++);
  if (dut->commit_wb == 1)
    set_state();
}

// simulate a reset
void reset(int n)
{
  dut->clk = 0;
  dut->rstn = 0;
  dut->eval();
  while (n-- > 0)
  {
    single_cycle();
  }
  dut->rstn = 1;
  dut->eval();
}

// check if the program should end
inline bool test_break()
{
  return dut->inst == 0x00100073U;
}

static void statistic()
{
  Log("total guest instructions = %ld", g_nr_guest_inst);
#ifdef TEST_CACHE_MISS_RATE
  print_cache_info();
#endif
  
}

void device_update();
// init the running state of our simulator
SimState sim_state = {.state = SIM_STOP};

// execute n instructions
void cpu_exec(unsigned int n)
{
  switch (sim_state.state)
  {
  case SIM_END:
  case SIM_ABORT:
  case SIM_QUIT:
    printf("Program execution has ended. To restart the program, exit NPC and run again.\n");
    return;
  default:
    sim_state.state = SIM_RUNNING;
  }

  bool npc_cpu_uncache_pre = 0;
  while (n--)
  {

    // q.enqueue(dut->pc_cur, dut->inst);

    // execute single instruction
    if (test_break())
    {
      // set the end state
      sim_state.halt_pc = dut->pc_cur;
      sim_state.halt_ret = cpu_gpr[10];
      sim_state.state = SIM_END;
      break;
    }

    if (dut->commit_wb)
    {
      q.enqueue(dut->pc_cur, dut->inst);
      
      if (npc_cpu_uncache_pre)
      {
        difftest_sync();
      }
      difftest_step();

      g_nr_guest_inst++;
      npc_cpu_uncache_pre = dut->uncache_read_wb;
    }
    // your cpu step a cycle
    single_cycle();

#ifdef DEVICE
    device_update();
#endif
    if (sim_state.state != SIM_RUNNING){
      break;
    }
  }

  switch (sim_state.state)
  {
  case SIM_RUNNING:
    sim_state.state = SIM_STOP;
    break;
  case SIM_END:
  case SIM_ABORT:
    Log("sim: %s at pc = " FMT_WORD,
        (sim_state.state == SIM_ABORT ? ANSI_FMT("ABORT", ANSI_FG_RED) : (sim_state.halt_ret == 0 ? ANSI_FMT("HIT GOOD TRAP", ANSI_FG_GREEN) : ANSI_FMT("HIT BAD TRAP", ANSI_FG_RED))),
        sim_state.halt_pc);
    q.display();
    break;
    // fall through
  case SIM_QUIT:
    statistic();
    break;
  }
}

static const char *regs[] = {
    "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
    "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
    "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
    "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

// map the name of reg to its value
word_t isa_reg_str2val(const char *s, bool *success)
{
  ;
  if (!strcmp(s, "pc"))
  {
    *success = true;
    return dut->pc_cur;
  }
  for (int i = 0; i < 32; i++)
  {
    if (!strcmp(s, regs[i]))
    {
      *success = true;
      return cpu_gpr[i];
    }
  }
  *success = false;
  return 0;
}

// set cpu_gpr point to your cpu's gpr
extern "C" void set_gpr_ptr(const svOpenArrayHandle r)
{
  cpu_gpr = (uint32_t *)(((VerilatedDpiOpenVar *)r)->datap());
}
// set the pointers pint to you cpu's csr
extern "C" void set_csr_ptr(const svOpenArrayHandle mstatus, const svOpenArrayHandle mtvec, const svOpenArrayHandle mepc, const svOpenArrayHandle mcause)
{
  cpu_mstatus = (uint32_t *)(((VerilatedDpiOpenVar *)mstatus)->datap());
  cpu_mtvec = (uint32_t *)(((VerilatedDpiOpenVar *)mtvec)->datap());
  cpu_mepc = (uint32_t *)(((VerilatedDpiOpenVar *)mepc)->datap());
  cpu_mcause = (uint32_t *)(((VerilatedDpiOpenVar *)mcause)->datap());
}

void isa_reg_display()
{
  for (int i = 0; i < 32; i++)
  {
    printf("gpr[%d](%s) = 0x%x", i, regs[i], cpu_gpr[i]);
    if (i % 4 == 3)
      printf("\n");
    else
      printf("\t");
  }
}

void print_itrace(unsigned int n)
{
  // Lab2 HINT: you can implement this function to help you print the instruction trace
  q.display(n);
}

void print_cache_info()
{
  if (dut->total_icache_access != 0) {
    double miss_rate = (double)dut->total_icache_miss / dut->total_icache_access * 100;
    Log("total icache access = %d, miss = %d, miss rate = %.2f%%", dut->total_icache_access, dut->total_icache_miss, miss_rate);
  } else {
    Log("icache miss rate = N/A (no accesses)");
  }
  if (dut->total_dcache_read_access != 0) {
    double read_miss_rate = (double)dut->total_dcache_read_miss / dut->total_dcache_read_access * 100;
    Log("total dcache read access = %d, miss = %d,read miss rate = %.2f%%", dut->total_dcache_read_access, dut->total_dcache_read_miss, read_miss_rate);
  } else {
    Log("dcache miss rate = N/A (no accesses)");
  }
  if (dut->total_dcache_write_access != 0) {
    double write_miss_rate = (double)dut->total_dcache_write_miss / dut->total_dcache_write_access * 100;
    Log("total dcache write access = %d, miss = %d, write miss rate = %.2f%%", dut->total_dcache_write_access, dut->total_dcache_write_miss, write_miss_rate);
  } else {
    Log("dcache miss rate = N/A (no accesses)");
  }
}

