/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static size_t buf_len = 0;
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

static uint32_t choose(uint32_t n) {
  return rand() % n;
}

static void append_str(const char *s) {
  size_t len = strlen(s);
  assert(buf_len + len < sizeof(buf));
  memcpy(buf + buf_len, s, len);
  buf_len += len;
  buf[buf_len] = '\0';
}

static void append_char(char c) {
  assert(buf_len + 1 < sizeof(buf));
  buf[buf_len ++] = c;
  buf[buf_len] = '\0';
}

static int32_t gen_num(bool nonzero) {
  int32_t val = choose(10);
  if (nonzero && val == 0) {
    val = choose(9) + 1;
  }

  char num[16];
  snprintf(num, sizeof(num), "%u", val);
  append_str(num);
  return val;
}

static void gen_spaces() {
  int n = choose(3);
  while (n -- > 0) {
    append_char(' ');
  }
}

static int32_t gen_expr(int depth) {
  if (depth > 3 || buf_len > sizeof(buf) - 128) {
    return gen_num(false);
  }

  switch (choose(3)) {
    case 0:
      return gen_num(false);

    case 1: {
      append_char('(');
      int32_t val = gen_expr(depth + 1);
      append_char(')');
      return val;
    }

    default: {
      int32_t lhs = gen_expr(depth + 1);
      gen_spaces();

      char ops[] = "+-*/";
      char op = ops[choose(4)];
      append_char(op);
      gen_spaces();

      int32_t rhs;
      if (op == '/') {
        // size_t snapshot = buf_len;
        // rhs = gen_expr(depth + 1);
        // if (rhs == 0) {
        //   buf_len = snapshot;
        //   buf[buf_len] = '\0';
        //   rhs = gen_num(true);
        // }
        rhs = gen_num(true);
      }
      else {
        rhs = gen_expr(depth + 1);
      }

      switch (op) {
        case '+': return lhs + rhs;
        case '-': return lhs - rhs;
        case '*': return lhs * rhs;
        case '/': return lhs / rhs;
        default: assert(0);
      }
    }
  }
}

static void gen_rand_expr() {
  buf[0] = '\0';
  buf_len = 0;
  gen_expr(0);
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  char code_file[64];
  char expr_file[64];
  char compile_cmd[256];
  snprintf(code_file, sizeof(code_file), "/tmp/.code.%d.c", getpid());
  snprintf(expr_file, sizeof(expr_file), "/tmp/.expr.%d", getpid());
  snprintf(compile_cmd, sizeof(compile_cmd), "gcc -fwrapv %s -o %s", code_file, expr_file);

  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop); // 提取一个整数到loop
  }
  int i;
  for (i = 0; i < loop; i ++) {
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen(code_file, "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system(compile_cmd);
    if (ret != 0) continue;

    fp = popen(expr_file, "r");
    assert(fp != NULL);

    unsigned result;
    ret = fscanf(fp, "%u", &result);
    assert(ret == 1);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  remove(code_file);
  remove(expr_file);
  return 0;
}
