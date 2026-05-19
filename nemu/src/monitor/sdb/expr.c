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

#include <isa.h>
#include <memory/vaddr.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

// 定义token类型
enum {
  TK_NOTYPE = 256, TK_EQ,

  /* TODO: Add more token types */
  TK_NUM,
  TK_HEX,
  TK_REG,
  TK_NEQ,
  TK_AND,
  TK_DEREF,


};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},             // spaces
  {"0[xX][0-9a-fA-F]+", TK_HEX}, // hex integer
  {"[0-9]+", TK_NUM},            // decimal integer
  {"\\$[a-zA-Z0-9]+", TK_REG},   // register
  {"==", TK_EQ},                 // equal
  {"!=", TK_NEQ},                // not equal
  {"&&", TK_AND},                // and
  {"\\+", '+'},                  // plus
  {"-", '-'},           // minus
  {"\\*", '*'},         // multiply
  {"/", '/'},           // divide

  {"\\(", '('},         // Left parenthesis
  {"\\)", ')'},         // right parenthesis
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[65536] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

// 求出待求值表达式的的token，存储到tokens[]数组里面
static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        // Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
        //     i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        assert(nr_token < ARRLEN(tokens));
        switch (rules[i].token_type) {
          case TK_NOTYPE:
            break;
          // 下面三种情况str不为空
          case TK_NUM:
          case TK_HEX:
          case TK_REG:
          // 超过32位字符断言失败
            assert(substr_len < sizeof(tokens[nr_token].str));
            tokens[nr_token].type = rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            // 加上结束标志\0
            tokens[nr_token].str[substr_len] = '\0';

            nr_token ++;
            break;
          
          default: 
            // TODO();
            tokens[nr_token].type = rules[i].token_type; 
            tokens[nr_token].str[0] = '\0';
            nr_token ++;
            break;
        }
        // for (int j = 0; j < nr_token; j ++) {
        //   printf("tokens[%d]: type = %d, str = %s\n",
        //   j, tokens[j].type, tokens[j].str);
        // }

        break;
      }
    }
    // 没有找到匹配的TOKEN
    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

// 把表达式开头或运算符后的 * 识别成解引用
static void mark_unary_tokens() {
  for (int i = 0; i < nr_token; i ++) {
    if (tokens[i].type == '*' &&
        (i == 0 ||
         tokens[i - 1].type == '(' ||
         tokens[i - 1].type == '+' ||
         tokens[i - 1].type == '-' ||
         tokens[i - 1].type == '*' ||
         tokens[i - 1].type == '/' ||
         tokens[i - 1].type == TK_EQ ||
         tokens[i - 1].type == TK_NEQ ||
         tokens[i - 1].type == TK_AND ||
         tokens[i - 1].type == TK_DEREF)) {
      tokens[i].type = TK_DEREF;
    }
  }
}
// 表达式求值：
// 1、递归

// 判断是不是真括号
static bool check_parentheses(int p, int q) {
  if (tokens[p].type != '(' || tokens[q].type != ')') {
    return false;
  }
  // 记录第几层括号
  int level = 0;
  for (int i = p; i <=q; i++) {
    if (tokens[i].type == '(') {
      level ++;
    }
    else if (tokens[i].type == ')') {
      level --;
    }
    if (level == 0 && i != q) {
      return false;
    }
  }
  assert(level == 0);
  return true;
}

// 表达式求值
static int precedence(int type) {
  switch (type) {
    case TK_AND: return 1;
    case TK_EQ:
    case TK_NEQ: return 2;
    case '+':
    case '-': return 3;
    case '*':
    case '/': return 4;
    case TK_DEREF: return 5;
    default: return 0;
  }
}

static bool is_operator(int type) {
  return precedence(type) != 0;
}

static word_t eval(int p, int q) {
  // int value = 0;
  if (p > q) {
    /* Bad expression */
    assert(0);
    return 0;
  }
  else if (p == q) {
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
    */
    // 读取tokens[q]中存储的值
    if (tokens[q].type == TK_NUM) {
      return strtoul(tokens[q].str, NULL, 10);
    }
    else if (tokens[q].type == TK_HEX) {
      return strtoul(tokens[q].str, NULL, 16);
    }
    else if (tokens[q].type == TK_REG) {
      bool success = true;
      word_t val = isa_reg_str2val(tokens[q].str + 1, &success);
      assert(success);
      return val;
    }
    assert(0);
    return 0;
  }
  else if (check_parentheses(p, q) == true) {
    return eval(p + 1, q - 1);
  }
  else {
    /* We should do more things here. */
    // 找到主运算符位置
    int op = -1;
    int level = 0; // 记录括号层数
    for (int i = p; i <= q; i++) {
      if (tokens[i].type == '(') {
        level ++;
      }
      else if (tokens[i].type == ')') {
        level --;
      }
      else if (level == 0) {
        if (is_operator(tokens[i].type)) {
          if (op == -1) {
            op = i;
          }
          else {
            int cur_level = precedence(tokens[i].type);
            int pre_level = precedence(tokens[op].type);
            if (cur_level <= pre_level) {
              op = i;
            }
          }
        }
      }
    }
    assert(op != -1);

    if (tokens[op].type == TK_DEREF) {
      word_t addr = (word_t)eval(op + 1, q);
      return vaddr_read(addr, 4);
    }

    // 分成两个表达式
    word_t val1 = eval(p, op - 1);
    word_t val2 = eval(op + 1, q);
    switch(tokens[op].type) {
      case '+':
        return val1 + val2;
      case '-':
        return val1 - val2;
      case '*':
        return val1 * val2;
      case '/':
        assert(val2 != 0);
        return val1 / val2;
      case TK_EQ:
        return val1 == val2;
      case TK_NEQ:
        return val1 != val2;
      case TK_AND:
        return val1 && val2;
      default:
        assert(0);
    }
  }
}

word_t expr(char *e, bool *success) {
  // 切分token存到tokens[]数组里
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  // 区分tokens数组里面的乘号*和解引用*
  mark_unary_tokens();
  *success = true;

  /* TODO: Insert codes to evaluate the expression. */
  // TODO();
  return (word_t)eval(0, nr_token - 1);
  // *success = true;
  return 0;
}
