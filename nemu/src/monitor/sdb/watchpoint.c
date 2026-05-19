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

#include "sdb.h"
#include <utils.h>

#define NR_WP 32
#define WP_EXPR_LEN 128

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  char expr[WP_EXPR_LEN];
  word_t last_value;

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

static WP* new_wp() {
  // 如果没有指向wp_pool，说明没初始化，报错
  if (free_ == NULL) {
    printf("No free watchpoint.\n");
    return NULL;
  }

  WP *wp = free_;
  free_ = free_->next;

  wp->next = head;
  head = wp;

  return wp;
}

void free_wp(int no) {
  WP *prev = NULL;
  WP *cur = head;

  while (cur != NULL) {
    if (cur->NO == no) {
      if (prev == NULL) {
        head = cur->next;
      }
      else {
        prev->next = cur->next;
      }

      cur->next = free_;
      free_ = cur;
      printf("Delete watchpoint %d\n", no);
      return;
    }

    prev = cur;
    cur = cur->next;
  }
  // 没找到
  printf("No watchpoint %d\n", no);
}

void watchpoint_new(char *e) {
  bool success = true;
  word_t val = expr(e, &success);
  if (!success) {
    printf("Bad expression: %s\n", e);
    return;
  }

  WP *wp = new_wp();
  if (wp == NULL) {
    return;
  }

  strncpy(wp->expr, e, sizeof(wp->expr) - 1);
  wp->expr[sizeof(wp->expr) - 1] = '\0';
  wp->last_value = val;

  printf("Watchpoint %d: %s = " FMT_WORD "\n", wp->NO, wp->expr, wp->last_value);
}

void display_watchpoints() {
  if (head == NULL) {
    printf("No watchpoints.\n");
    return;
  }

  printf("Num\tValue\t\tExpr\n");
  for (WP *wp = head; wp != NULL; wp = wp->next) {
    printf("%d\t" FMT_WORD "\t%s\n", wp->NO, wp->last_value, wp->expr);
  }
}

bool check_watchpoints() {
  bool hit = false;

  for (WP *wp = head; wp != NULL; wp = wp->next) {
    bool success = true;
    word_t new_value = expr(wp->expr, &success);

    if (!success) {
      printf("Bad expression in watchpoint %d: %s\n", wp->NO, wp->expr);
      continue;
    }

    if (new_value != wp->last_value) {
      printf("Watchpoint %d triggered: %s\n", wp->NO, wp->expr);
      printf("Old value = " FMT_WORD "\n", wp->last_value);
      printf("New value = " FMT_WORD "\n", new_value);
      wp->last_value = new_value;
      nemu_state.state = NEMU_STOP;
      hit = true;
    }
  }

  return hit;
}
