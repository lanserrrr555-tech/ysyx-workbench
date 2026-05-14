// // // 以下为伪代码

// // #include <stdio.h>
// // #include <stdlib.h>
// // #include <assert.h>

// // while (???) {
// //   int a = rand() & 1;
// //   int b = rand() & 1;
// //   top->a = a;
// //   top->b = b;
// //   top->eval();
// //   printf("a = %d, b = %d, f = %d\n", a, b, top->f);
// //   assert(top->f == (a ^ b));
// // }
// #include <stdio.h>
// #include <stdlib.h>
// #include <assert.h>
// #include <verilated.h>
// #include "Vtop.h"

// int main(int argc, char** argv) {
//     VerilatedContext* const contextp = new VerilatedContext;
//     contextp->commandArgs(argc, argv);
//     Vtop* const top = new Vtop{contextp};

//     // 主循环：跑 10 次随机测试
//     int count = 10;
//     while (count-- > 0 && !contextp->gotFinish()) {
//         int a = rand() & 1;          // 取最低位，得到 0 或 1
//         int b = rand() & 1;
//         top->a = a;                  // 驱动输入
//         top->b = b;
//         top->eval();                 // 重新求值组合逻辑
//         printf("a = %d, b = %d, f = %d\n", a, b, top->f);
//         assert(top->f == (a ^ b));   // 验证结果
//     }

//     top->final();
//     delete top;
//     delete contextp;
//     return 0;
// }
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <verilated.h>
#include <verilated_vcd_c.h>   // 新增：VCD 追踪头文件
#include "VSW2_top.h"

int main(int argc, char** argv) {
    VerilatedContext* contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);

    VSW2_top* top = new VSW2_top{contextp};

    VerilatedVcdC* tfp = nullptr;

    // 启用追踪
    contextp->traceEverOn(true);
    tfp = new VerilatedVcdC;
    top->trace(tfp, 0);            // 0 = 只追踪顶层；用1/2追踪更深层
    tfp->open("wave.vcd");         // 输出文件名

    // ===== 关键：限制循环次数，防止磁盘爆掉 =====
    int count = 10;
    while (count-- > 0) {
        int a = rand() & 1;
        int b = rand() & 1;
        top->a = a;
        top->b = b;
        top->eval();
        contextp->timeInc(1);
        tfp->dump(contextp->time());
        printf("t=%ld a=%d b=%d f=%d\n",
               contextp->time(), a, b, top->f);
        assert(top->f == (a ^ b));
    }

    tfp->close();                  // 必须关闭，否则文件可能不完整
    delete top;
    delete contextp;
    return 0;
}

